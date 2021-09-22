#include "MainHeader.h"
#ifdef ENABLE_BLUETOOTH_LOGGING
BluetoothSerial SerialBT;
#endif
// Only one file is allowed to include TaskScheduler
#define _TASK_STATUS_REQUEST
#include "TaskScheduler.h"

#include "ClientStorage.h"
#include "Task.h"

#define BEACON_INTERVAL (1000 * TASK_MILLISECOND)

void onRecvFromClient(const uint8_t *mac_addr, const uint8_t *data, int data_len);

esp_now_peer_info_t broadcastPeer{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
Scheduler scheduler;
BeaconPacket broadcastData;

void setup()
{
  Serial.begin(115200);
#ifdef ENABLE_BLUETOOTH_LOGGING
  SerialBT.begin("ESP32-LOG");
#endif
#if defined(ENABLE_LOGGING) || defined(ENABLE_BLUETOOTH_LOGGING)
  Log.begin(LOG_LEVEL_VERBOSE, &LOGGER);
#endif

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

  broadcastPeer.channel = WIFI_CHANNEL;
  broadcastPeer.encrypt = false;

  client_storage::init();

  beacon::init(&scheduler);
  beacon::setPeer(&broadcastPeer);
  beacon::setData((uint8_t *)&broadcastData, sizeof(broadcastData));
  beacon::task.setInterval(BEACON_INTERVAL);

  beacon::task.enable();
}

void loop()
{
  scheduler.execute();
}

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