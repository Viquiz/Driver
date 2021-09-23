#include "Task.hpp"

namespace beacon
{
    void callback(TimerHandle_t timer)
    {
        updatePacket();
        esp_now_send(peer.peer_addr, (uint8_t *)&packet, sizeof(packet));
    }

    void updatePacket()
    {
        // Update unanswered
        xQueueReceive(packetUpdateHandler, (void *)&packet.unanswered, 0);
        // Update time remain
        // TODO: timeLimit + timeStarted - millis()
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
        esp_err_t esp_err = esp_now_add_peer(&peer);
        if (esp_err != ESP_OK ||
            (esp_err == ESP_ERR_ESPNOW_EXIST && !ignorePeerExisted))
        {
            Log.errorln("Add beacon peer failed. Code 0x%x", esp_err);
            return false;
        }
        return true;
    }

    bool delPeer(bool ignorePeerNotFound)
    {
        esp_err_t esp_err = esp_now_del_peer(peer.peer_addr);
        if (esp_err != ESP_OK ||
            (esp_err == ESP_ERR_ESPNOW_NOT_FOUND && !ignorePeerNotFound))
        {
            Log.errorln("Delete beacon peer failed. Code 0x%x", esp_err);
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
