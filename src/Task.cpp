#include <ArduinoJson.h>
#include "Task.hpp"

Beacon::Beacon(esp_now_peer_info_t peer) : peer(peer) {}

bool Beacon::create(const char *const pcTimerName,
                    const TickType_t xTimerPeriodInTicks,
                    bool uxAutoReload)
{
    return (this->handler = xTimerCreate(pcTimerName,
                                         xTimerPeriodInTicks,
                                         uxAutoReload ? pdTRUE : pdFALSE,
                                         this,
                                         callbackAdapter)) != NULL;
}

esp_err_t Beacon::onStart()
{
    return esp_now_add_peer(&this->peer);
}

esp_err_t Beacon::onStop()
{
    return esp_now_del_peer(&this->peer);
}

bool Beacon::start(TickType_t xTicksToWait)
{
    return (onStart() == ESP_OK) &&
           (xTimerStart(this->handler, xTicksToWait) == pdTRUE
                ? true
                : false);
}

bool Beacon::stop(TickType_t xTicksToWait)
{
    return (xTimerStop(this->handler, xTicksToWait) == pdTRUE ? true : false) &&
           (onStop() == ESP_OK);
}

void Beacon::callback(TimerHandle_t xTimer)
{
}

void Beacon::callbackAdapter(TimerHandle_t xTimer)
{
    Beacon *b = static_cast<Beacon *>(pvTimerGetTimerID(xTimer));
    b->callback(xTimer);
}