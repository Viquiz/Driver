#ifndef _BEACON_
#define _BEACON_
#include "MainHeader.h"

namespace beacon
{
    extern Task task;

    void init(unsigned long interval,
              Scheduler *,
              const esp_now_peer_info_t *,
              const uint8_t *data, size_t len);
    void emit();
    bool onEnable();
    void onDisable();
} // namespace beacon
#endif