#include "Task.h"
#include <ArduinoJson.h>
namespace beacon
{
    Task task;
    const esp_now_peer_info_t *peer;
    const uint8_t *data;
    size_t len;

    void init(Scheduler *scheduler)
    {
        task.setCallback(emit);
        task.setOnEnable(onEnable);
        task.setOnDisable(onDisable);
        scheduler->addTask(task);
    }

    void setPeer(const esp_now_peer_info_t *peer_info)
    {
        if (peer) // delete old peer first
            esp_now_del_peer(peer->peer_addr);
        peer = peer_info;
    }

    void setData(const uint8_t *data, size_t len)
    {
        beacon::data = data;
        beacon::len = len;
    }

    void emit()
    {
        esp_now_send(peer->peer_addr, data, len);
    }

    bool onEnable()
    {
        esp_err_t err = esp_now_add_peer(peer);
        return err == ESP_OK || err == ESP_ERR_ESPNOW_EXIST;
    }

    void onDisable()
    {
        esp_now_del_peer(peer->peer_addr);
    }
} // namespace beacon

namespace serial_poll
{
    Task task;

    void init(Scheduler *scheduler)
    {
        task.setCallback(callback);
        scheduler->addTask(task);
    }

    void callback()
    {
        if (Serial.available())
        {
            StaticJsonDocument<SERIAL_BUFFER_SIZE> doc;
            DeserializationError err = deserializeJson(doc, Serial);
            if (err)
            {
                LOG_ERROR("deserializeJson() failed: %s", err.c_str());
                return;
            }
            message_t type = doc["type"];
            switch (type)
            {
            case message_t::NO_TYPE_FOUND:
                LOG_ERROR("No \"type\" in JSON message");
                break;
            case message_t::RESPOND_REG_CLIENT:
            
                break;
            default:
                LOG_ERROR("Unknown type for JSON message");
                break;
            }
        }
    }
} // namespace serial_poll
