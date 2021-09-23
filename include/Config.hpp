#ifndef _CONFIG_
#define _CONFIG_

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "ServerHostProtocol.hpp"
#include "ServerClientProtocol/ServerClientProtocol.hpp"

// #define DISABLE_LOGGING
#include "SerialLog.hpp"

#define WIFI_CHANNEL 0
#define BROADCAST_MAC {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

#define BEACON_TIMER_NAME "beacon"
#define BEACON_TIMER_ID 0
#define BEACON_MILLI_PERIOD 500
#define BEACON_QUEUE_LENGTH 10
#define BEACON_QUEUE_ITEM_SIZE sizeof(uint8_t)

#define SERIAL_BUFFER_SIZE 256
#define SERIAL_POLL_MILLI 500

#endif // !_CONFIG_