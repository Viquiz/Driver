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

#define SERIAL_BUFFER_SIZE 256

#define BEACON_MILLI_INTERVAL 500

#endif // !_CONFIG_