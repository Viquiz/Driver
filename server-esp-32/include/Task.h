#ifndef _TASK_
#define _TASK_
#include "MainHeader.h"

namespace beacon
{
    extern Task task;

    void init(Scheduler *);
    void setInterval(unsigned long);
    void setPeer(const esp_now_peer_info_t *);
    void setData(const uint8_t *data, size_t len);

    void emit();
    bool onEnable();
    void onDisable();
} // namespace beacon
#endif