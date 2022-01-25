#ifndef _TASK_
#define _TASK_
#include "Config.hpp"

namespace beacon
{
    extern BeaconPacket packet;
    extern esp_now_peer_info_t peer;
    extern TimerHandle_t timerHandler;
    extern QueueHandle_t packetUpdateHandler;

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

namespace serial_rx
{
    extern TaskHandle_t taskHandler;

    bool create(const char *const name = SERIAL_RX_NAME,
                uint32_t stackDepth = SERIAL_RX_STACK,
                void *const param = NULL,
                UBaseType_t priority = SERIAL_RX_PRIORITY);

    bool createPinnedToCore(BaseType_t coreID,
                            const char *const name = SERIAL_RX_NAME,
                            uint32_t stackDepth = SERIAL_RX_STACK,
                            void *const param = NULL,
                            UBaseType_t priority = SERIAL_RX_PRIORITY);
} // namespace serial_rx

namespace serial_tx
{
    /**
   * TODO: Consider 2 approach:
   * 1. Send to serial_tx (better data seperation, but data are passed by value)
   * 2. Handle it directly here (too much work for Wifi task? Especially serializeJson())
   */

    // TaskHandle_t taskHandler = NULL;
    // bool create(const char *const name = SERIAL_TX_NAME,
    //             uint32_t stackDepth = SERIAL_TX_STACK,
    //             void *const param = NULL,
    //             UBaseType_t priority = SERIAL_TX_PRIORITY,
    //             UBaseType_t queueLength = SERIAL_TX_QUEUE_LENGTH,
    //             UBaseType_t queueItemSize = SERIAL_TX_ITEM_SIZE);
    // bool createPinnedToCore(BaseType_t coreID,
    //                         const char *const name = SERIAL_TX_NAME,
    //                         uint32_t stackDepth = SERIAL_TX_STACK,
    //                         void *const param = NULL,
    //                         UBaseType_t priority = SERIAL_TX_PRIORITY,
    //                         UBaseType_t queueLength = SERIAL_TX_QUEUE_LENGTH,
    //                         UBaseType_t queueItemSize = SERIAL_TX_ITEM_SIZE);

    void sendRequestRegister(const uint8_t *addr, const RequestRegisterPacket *data);
    void sendAnswer(const uint8_t *addr, const AnswPacket *data);
}

#endif //!_TASK_