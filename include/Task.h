#ifndef _TASK_
#define _TASK_
#include "MainHeader.h"

namespace beacon
{
    extern Task task;

    void init(Scheduler *);
    void setPeer(const esp_now_peer_info_t *, bool remove_old_peer = true);
    void setData(const uint8_t *data, size_t len);

    void emit();
    bool onEnable();
    void onDisable();
} // namespace beacon

namespace serial_poll
{
    extern Task task;

    void init(Scheduler *);
    void callback();
} // namespace serial_poll

#endif