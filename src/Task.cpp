#include <ArduinoJson.h>
#include "Task.hpp"

namespace beacon
{
    BeaconPacket packet;
    esp_now_peer_info_t peer;
    TimerHandle_t timerHandler = NULL;
    QueueHandle_t packetUpdateHandler = NULL;

    void updatePacket()
    {
        // Update unanswered
        xQueueReceive(packetUpdateHandler, (void *)&packet.unanswered, 0);
        // Update time remain
        // TODO: timeLimit + timeStarted - millis()
    }

    void callback(TimerHandle_t timer)
    {
        updatePacket();
        esp_now_send(peer.peer_addr, (uint8_t *)&packet, sizeof(packet));
    }

    bool create(const char *timerName,
                TickType_t timerPeriodInTicks,
                bool autoReload,
                void *const timerID,
                UBaseType_t queueLength,
                UBaseType_t queueItemSize)
    {
        timerHandler = xTimerCreate(timerName,
                                    timerPeriodInTicks,
                                    autoReload,
                                    timerID,
                                    callback);
        packetUpdateHandler = xQueueCreate(queueLength,
                                           queueItemSize);
        if (timerHandler == NULL)
            Log.errorln("Create beacon failed");
        if (packetUpdateHandler == NULL)
            Log.errorln("Create beacon packet queue failed");
        return timerHandler != NULL ||
               packetUpdateHandler != NULL;
    }

    bool addPeer(bool ignorePeerExisted)
    {
        esp_err_t err = esp_now_add_peer(&peer);
        if (err != ESP_OK ||
            (err == ESP_ERR_ESPNOW_EXIST && !ignorePeerExisted))
        {
            Log.errorln("Add beacon peer failed. Code 0x%x", err);
            return false;
        }
        return true;
    }

    bool delPeer(bool ignorePeerNotFound)
    {
        esp_err_t err = esp_now_del_peer(peer.peer_addr);
        if (err != ESP_OK ||
            (err == ESP_ERR_ESPNOW_NOT_FOUND && !ignorePeerNotFound))
        {
            Log.errorln("Delete beacon peer failed. Code 0x%x", err);
            return false;
        }
        return true;
    }

    bool start(TickType_t ticksToWait,
               bool addPeerToList,
               bool ignorePeerExisted)
    {
        if (addPeerToList && !addPeer(ignorePeerExisted))
            return false;
        return xTimerStart(timerHandler, ticksToWait);
    }

    bool startFromISR(bool higherPriorityTaskWoken,
                      bool addPeerToList,
                      bool ignorePeerExisted)
    {
        if (addPeerToList && !addPeer(ignorePeerExisted))
            return false;
        return xTimerStartFromISR(timerHandler,
                                  (BaseType_t *)&higherPriorityTaskWoken);
    }

    bool stop(TickType_t ticksToWait,
              bool delPeerFromList,
              bool ignorePeerNotFound)
    {
        if (delPeerFromList && !delPeer(ignorePeerNotFound))
            return false;
        return xTimerStop(timerHandler, ticksToWait);
    }
} // namespace beacon

namespace serial_rx_poll
{
    TaskHandle_t taskHandler = NULL;

    void callback(void *param)
    {
        while (true)
        {
            if (Serial.available())
            {
                StaticJsonDocument<SERIAL_BUFFER_SIZE> doc;
                DeserializationError err = deserializeJson(doc, Serial);
                if (err)
                {
                    Log.errorln("deserializeJson(). %s", err.c_str());
                    return;
                }
                message_t type = doc["type"];
                switch (type)
                {
                case message_t::RESPOND_REG_CLIENT:
                    // TODO: xQueueSend() to respond client task
                    break;
                default:
                    Log.errorln("Invalid \"type\" in JSON message");
                    break;
                }
            }
            vTaskDelay(SERIAL_RX_MILLI_DELAY / portTICK_PERIOD_MS);
        }
    }

    bool create(const char *const name,
                uint32_t stackDepth,
                void *const param,
                UBaseType_t priority)
    {
        return xTaskCreate(callback,
                           name,
                           stackDepth,
                           param,
                           priority,
                           &taskHandler);
    }

    bool createPinnedToCore(BaseType_t coreID,
                            const char *const name,
                            uint32_t stackDepth,
                            void *const param,
                            UBaseType_t priority)
    {
        return xTaskCreatePinnedToCore(callback,
                                       name,
                                       stackDepth,
                                       param,
                                       priority,
                                       &taskHandler,
                                       coreID);
    }
} // namespace serial_rx_poll

namespace serial_tx
{
    /**
     * NOTE: When you pass a Stream to serializeJson(), it writes the JSON to 
     * the stream but doesn’t print anything to the serial port, which makes 
     * troubleshooting difficult.
     */

    void setTypeAndAddr(JsonDocument &doc, message_t type, const uint8_t *addr)
    {
        doc[JSON_TYPE_KEY] = type;
        JsonArray jsonAddr = doc.createNestedArray(JSON_ADDR_KEY);
        for (uint8_t i = 0; i < 6; i++)
        {
            jsonAddr.add(addr[i]);
        }
    }

    void sendRequestRegister(const uint8_t *addr, const RequestRegisterPacket *data)
    {
        const int capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(6);
        StaticJsonDocument<capacity> doc;
        setTypeAndAddr(doc, message_t::REG_CLIENT, addr);
        serializeJson(doc, Serial);
    }

    void sendAnsw(const uint8_t *addr, const AnswPacket *data)
    {
        const int capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(6);
        StaticJsonDocument<capacity> doc;
        setTypeAndAddr(doc, message_t::CLIENT_ANSWER, addr);
        doc[JSON_ANSW_KEY] = data->clientAnsw;
        serializeJson(doc, Serial);
    }
} // namespace serial_tx
