#ifndef _CLIENT_STORAGE_
#define _CLIENT_STORAGE_
#include "MainHeader.h"

#define CLIENT_MAX_NUM 50

namespace client_storage
{
    void init();
    bool insert(const uint8_t *macAddr, int idx);
    esp_now_peer_info_t *get(int idx);
} // namespace client_storage


#endif // !_CLIENT_STORAGE_