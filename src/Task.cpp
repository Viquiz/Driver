#include <ArduinoJson.h>
#include "Task.hpp"

namespace beacon
{
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
