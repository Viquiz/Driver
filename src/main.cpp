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

void *memcpyAddr(uint8_t *dest, const uint8_t *src, size_t n)
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

void sendAnswToSerial(const uint8_t *addr, const AnswPacket *data)
{
  /**
   * TODO: Consider 2 approach:
   * 1. Send to serial_tx (better data seperation, but data are passed by value)
   * 2. Handle it directly here (too much work for Wifi task?)
   */
}

void sendReqRegToSerial(const uint8_t *addr, const RequestRegisterPacket *data)
{
  /**
   * TODO: Consider 2 approach:
   * 1. Send to serial_tx (better data seperation, but data are passed by value)
   * 2. Handle it directly here (too much work for Wifi task?)
   */
}

void respondAnswToClient(const uint8_t *addr)
{
  esp_now_peer_info_t peer;
  memcpyAddr(peer.peer_addr, addr);
  peer.channel = WIFI_CHANNEL;
  peer.encrypt = ESPNOW_ENCRYPT;
  esp_now_send_once(&peer, (uint8_t *)&game.quiz.correctAnsw, sizeof(game.quiz.correctAnsw));
}

void onRecvFromClient(const uint8_t *peer_addr, const uint8_t *data, int data_len)
{
  // Unpack data
  switch (((BasePacket *)data)->type)
  {
  case packet_t::RECV_ANSW:
  {
    sendAnswToSerial(peer_addr, (AnswPacket *)data);
    respondAnswToClient(peer_addr);
    break;
  }
  case packet_t::REQ_REG:
  {
    sendReqRegToSerial(peer_addr, (RequestRegisterPacket *)data);
    break;
  }
  default:
    break;
  }
}

void loop() {}