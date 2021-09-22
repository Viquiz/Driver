#ifndef _TASK_
#define _TASK_
#include "Config.hpp"

class Beacon
{
public:
    Beacon(esp_now_peer_info_t, uint8_t *data);
    bool create(const char *const pcTimerName,
                const TickType_t xTimerPeriodInTicks,
                bool uxAutoReload);
    bool start(TickType_t xTicksToWait);
    bool stop(TickType_t xTicksToWait);

    const TimerHandle_t getHandler() const;

private:
    TimerHandle_t handler = NULL;
    esp_now_peer_info_t peer;

    esp_err_t onStart();
    esp_err_t onStop();

    void callback(TimerHandle_t);
    static void callbackAdapter(TimerHandle_t);
};

#endif //!_TASK_