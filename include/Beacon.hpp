#ifndef _TASK_BEACON_
#define _TASK_BEACON_
#include "Config.hpp"

struct BeaconHandle
{
    BeaconPacket packet;
    esp_now_peer_info_t peer;
    TimerHandle_t timer;
    QueueHandle_t packetUpdateQueue;
};

BeaconHandle *beaconCreate(BeaconHandle *bh,
                           const char *timerName,
                           TickType_t timerPeriodInTicks = pdMS_TO_TICKS(BEACON_MILLI_PERIOD),
                           bool autoReload = true,
                           UBaseType_t queueLength = BEACON_QUEUE_LENGTH,
                           UBaseType_t queueItemSize = BEACON_QUEUE_ITEM_SIZE);

bool beaconStart(BeaconHandle *bh,
                 TickType_t ticksToWait,
                 bool addPeerToList,
                 bool ignorePeerExisted);

bool beaconStartFromISR(BeaconHandle *bh,
                        bool higherPriorityTaskWoken,
                        bool addPeerToList,
                        bool ignorePeerExisted);

bool beaconStop(BeaconHandle *bh,
                TickType_t ticksToWait,
                bool delPeerFromList,
                bool ignorePeerNotFound);

#endif //!_TASK_BEACON_