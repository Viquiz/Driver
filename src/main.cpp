#include "Config.hpp"
#ifdef ENABLE_BLUETOOTH_LOGGING
BluetoothSerial SerialBT;
#endif

esp_now_peer_info_t broadcastPeer{BROADCAST_MAC};
BeaconPacket broadcastData;

void onRecvFromClient(const uint8_t *mac_addr, const uint8_t *data, int data_len);

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
  broadcastPeer.encrypt = false;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK)
  {
    Log.fatalln("[ERROR] Initialize ESP-NOW failed");
    delay(500);
    ESP.restart();
  }
  Log.verboseln("MAC: %s", WiFi.macAddress().c_str());
  esp_now_register_recv_cb(&onRecvFromClient);

  // client_storage::init();
}

void loop() {}

void onRecvFromClient(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  // Unpack data
  switch (((BasePacket *)data)->type)
  {
  case packet_t::RECV_ANSW:
    // Receive client's answer to the question
    break;
  default:
    break;
  }
}