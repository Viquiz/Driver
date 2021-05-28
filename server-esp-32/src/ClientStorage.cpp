#include "ClientStorage.h"
// This is a copy of client list stored in host device
namespace client_storage
{
    esp_now_peer_info_t *arr = new esp_now_peer_info_t[CLIENT_MAX_NUM];

    void init()
    {
        for (int i = 0; i < CLIENT_MAX_NUM; i++)
        {
            esp_now_peer_info_t *client = &arr[i];
            client->channel = CHANNEL;
            client->encrypt = false;
        }
    }

    bool insert(const uint8_t *macAddr, int idx)
    {
        if (idx < CLIENT_MAX_NUM)
        {
            memcpy(arr[idx].peer_addr, macAddr, 6);
            return true;
        }
        return false;
    }

    esp_now_peer_info_t *get(int idx)
    {
        return &arr[idx];
    }

} // namespace client_storage
