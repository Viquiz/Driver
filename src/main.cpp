#include "Config.hpp"
#ifdef ENABLE_BLUETOOTH_LOGGING
BluetoothSerial SerialBT;
#endif

esp_now_peer_info_t broadcastPeer{BROADCAST_MAC};
Game game{server_state_t::IDLE};

void *memcpyAddr(uint8_t *dest, const uint8_t *src, size_t n = 6);
esp_err_t esp_now_send_once(const esp_now_peer_info_t *peer, const uint8_t *data, size_t len);
void onRecvFromClient(const uint8_t *peer_addr, const uint8_t *data, int data_len);

void setup()
{
  Serial.begin(115200);
#ifndef DISABLE_LOGGING
#ifdef BLUETOOTH_LOGGING
  SerialBT.begin("ESP32-LOG");
#endif
  Log.begin(LOG_LEVEL_VERBOSE, &LOGGER);
#endif
  broadcastPeer.channel = WIFI_CHANNEL;
  broadcastPeer.encrypt = ESPNOW_ENCRYPT;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK)
  {
    Log.errorln("Initialize ESP-NOW failed");
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ESP.restart();
  }
  Log.verboseln("MAC: %s", WiFi.macAddress().c_str());
  esp_now_register_recv_cb(&onRecvFromClient);
}

void *memcpyAddr(uint8_t *dest, const uint8_t *src, size_t n = 6)
{
  return memcpy(dest, src, n);
}

esp_err_t esp_now_send_once(const esp_now_peer_info_t *peer, const uint8_t *data, size_t len)
{
  esp_now_add_peer(peer);
  esp_err_t err = esp_now_send(peer->peer_addr, data, len);
  esp_now_del_peer(peer->peer_addr);
  return err;
}

void onRecvFromClient(const uint8_t *peer_addr, const uint8_t *data, int data_len)
{
  // Unpack data
  switch (((BasePacket *)data)->type)
  {
  case packet_t::RECV_ANSW:
  {
    AnswPacket *packet = (AnswPacket *)data;
    // TODO: Send AnswPacket to serial_tx task
    esp_now_peer_info_t peer;
    memcpyAddr(peer.peer_addr, peer_addr);
    peer.channel = WIFI_CHANNEL;
    peer.encrypt = ESPNOW_ENCRYPT;
    // Reply client with RespondAnswPacket
    esp_now_send_once(&peer, (uint8_t *)&game.quiz.correctAnsw, sizeof(game.quiz.correctAnsw));
    break;
  }
  case packet_t::REQ_REG:
  {
    // TODO: Foward to Host
  }
  default:
    break;
  }
}

void loop() {}