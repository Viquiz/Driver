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
                TickType_t timerPeriodInTicks = pdMS_TO_TICKS(BEACON_MILLI_PERIOD),
                bool autoReload = true,
                void *const timerID = (void *)BEACON_TIMER_ID,
                UBaseType_t queueLength = BEACON_QUEUE_LENGTH,
                UBaseType_t queueItemSize = BEACON_QUEUE_ITEM_SIZE);

    bool start(TickType_t ticksToWait,
               bool addPeerToList = true,
               bool ignorePeerExisted = true);

    bool startFromISR(bool higherPriorityTaskWoken,
                      bool addPeerToList = true,
                      bool ignorePeerExisted = true);

    bool stop(TickType_t ticksToWait,
              bool delPeerFromList = true,
              bool ignorePeerNotFound = true);
} // namespace beacon

namespace serial_rx_poll
{
    TaskHandle_t taskHandler = NULL;

    bool create(const char *const name = SERIAL_RX_NAME,
                uint32_t stackDepth = SERIAL_RX_STACK,
                void *const param = NULL,
                UBaseType_t priority = SERIAL_RX_PRIORITY);

    bool createPinnedToCore(BaseType_t coreID,
                            const char *const name = SERIAL_RX_NAME,
                            uint32_t stackDepth = SERIAL_RX_STACK,
                            void *const param = NULL,
                            UBaseType_t priority = SERIAL_RX_PRIORITY);
} // namespace serial_rx_poll

namespace serial_tx
{
    TaskHandle_t taskHandler = NULL;

    bool create(const char *const name = SERIAL_TX_NAME,
                uint32_t stackDepth = SERIAL_TX_STACK,
                void *const param = NULL,
                UBaseType_t priority = SERIAL_TX_PRIORITY,
                UBaseType_t queueLength = SERIAL_TX_QUEUE_LENGTH,
                UBaseType_t queueItemSize = SERIAL_TX_ITEM_SIZE);

    bool createPinnedToCore(BaseType_t coreID,
                            const char *const name = SERIAL_TX_NAME,
                            uint32_t stackDepth = SERIAL_TX_STACK,
                            void *const param = NULL,
                            UBaseType_t priority = SERIAL_TX_PRIORITY,
                            UBaseType_t queueLength = SERIAL_TX_QUEUE_LENGTH,
                            UBaseType_t queueItemSize = SERIAL_TX_ITEM_SIZE);
}

#endif //!_TASK_