#include "Task.hpp"

namespace beacon
{
    bool create(const char *timerName,
                TickType_t timerPeriodInTicks,
                bool autoReload,
                void *const timerId,
                UBaseType_t queueLength,
                UBaseType_t queueItemSize)
    {
        timerHandler = xTimerCreate(timerName,
                                    timerPeriodInTicks,
                                    autoReload,
                                    timerId,
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

    bool start(bool addPeerToList, bool fromISR)
    {
        
    }
} // namespace beacon
