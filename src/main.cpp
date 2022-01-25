#include "Config.hpp"
#include "Task.hpp"
#ifdef BLUETOOTH_LOGGING
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

  if (!beacon::create())
  {
    ESP.restart();
  }
  beacon::start(portMAX_DELAY);
  // Don't know if I should use create or createPinnedToCore
  serial_rx::create();

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void *memcpyAddr(uint8_t *dest, const uint8_t *src, size_t n)
{
  return memcpy(dest, src, n);
}

/**
 * Add a peer to peer list, send the data, then immediately remove that peer from peer list
*/
esp_err_t esp_now_send_once(const esp_now_peer_info_t *peer, const uint8_t *data, size_t len)
{
  esp_now_add_peer(peer);
  esp_err_t err = esp_now_send(peer->peer_addr, data, len);
  esp_now_del_peer(peer->peer_addr);
  return err;
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
    serial_tx::sendAnswer(peer_addr, (AnswPacket *)data);
    respondAnswToClient(peer_addr);
    break;
  }
  case packet_t::REQ_REG:
  {
    serial_tx::sendRequestRegister(peer_addr, (RequestRegisterPacket *)data);
    break;
  }
  default:
    break;
  }
}

void loop() {}