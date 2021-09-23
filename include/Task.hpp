#ifndef _TASK_
#define _TASK_
#include "Config.hpp"

namespace beacon
{
    BeaconPacket packet;
    esp_now_peer_info_t peer;
    TimerHandle_t timerHandler = NULL;
    QueueHandle_t packetUpdateHandler = NULL;

    bool create(const char *timerName = BEACON_TIMER_NAME,
                TickType_t timerPeriodInTicks = BEACON_MILLI_PERIOD / portTICK_PERIOD_MS,
                bool autoReload = true,
                void *const timerId = (void *)BEACON_TIMER_ID,
                UBaseType_t queueLength = BEACON_QUEUE_LENGTH,
                UBaseType_t queueItemSize = BEACON_QUEUE_ITEM_SIZE);

    bool start(bool addPeerToList = true, bool fromISR = false);
    bool stop(bool delPeerFromList = true, bool fromISR = false);

    void callback(TimerHandle_t);
} // namespace beacon

#endif //!_TASK_