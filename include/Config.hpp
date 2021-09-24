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
#define BROADCAST_MAC                      \
    {                                      \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF \
    }

#define BEACON_TIMER_NAME "beacon"
#define BEACON_TIMER_ID 0
#define BEACON_MILLI_PERIOD 500
#define BEACON_QUEUE_LENGTH 10
#define BEACON_QUEUE_ITEM_SIZE sizeof(uint8_t)

#define SERIAL_BUFFER_SIZE 256

#define SERIAL_RX_NAME "serialRx"
#define SERIAL_RX_STACK configMINIMAL_STACK_SIZE + SERIAL_BUFFER_SIZE + 128
#define SERIAL_RX_PRIORITY 1
#define SERIAL_RX_MILLI_DELAY 500

#define SERIAL_TX_NAME "serialTx"
#define SERIAL_TX_STACK // TODO
#define SERIAL_TX_PRIORITY 3
#define SERIAL_TX_QUEUE_LENGTH 25
#define SERIAL_TX_ITEM_SIZE // TODO

struct Game
{
    server_state_t serverState;
    client_t clientSize;
    client_t unanswered;
    struct Quiz
    {
        uint8_t correctAnsw;
        uint32_t startTime;
        uint32_t endTime;
    } quiz;
};

#endif // !_CONFIG_