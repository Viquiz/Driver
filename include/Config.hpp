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
#define ESPNOW_ENCRYPT 0
#define BROADCAST_MAC                      \
    {                                      \
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF \
    }

#define BEACON_TIMER_NAME "beacon"
#define BEACON_TIMER_ID 0
#define BEACON_MILLI_PERIOD 500
#define BEACON_QUEUE_LENGTH 10
#define BEACON_QUEUE_ITEM_SIZE sizeof(BeaconPacket::unanswered)

#define SERIAL_BUFFER_SIZE 256

#define SERIAL_RX_NAME "serialRx"
#define SERIAL_RX_STACK configMINIMAL_STACK_SIZE + SERIAL_BUFFER_SIZE + 128
#define SERIAL_RX_PRIORITY 0
#define SERIAL_RX_MILLI_DELAY 500

#define SERIAL_TX_NAME "serialTx"
#define SERIAL_TX_STACK configMINIMAL_STACK_SIZE + SERIAL_BUFFER_SIZE + 128
#define SERIAL_TX_PRIORITY 2
#define SERIAL_TX_QUEUE_LENGTH 10
#define SERIAL_TX_ITEM_SIZE sizeof(SerialTxAdapter)

struct Game
{
    server_state_t serverState;
    client_t clientSize;
    client_t unanswered;
    struct Quiz
    {
        RespondAnswPacket correctAnsw;
        uint32_t startTime;
        uint32_t endTime;
    } quiz;
};

struct SerialTxAdapter
{
    message_t type;
    uint8_t clientAddr[6];
    // Only use when type = CLIENT_ANSWER
    btn_t clientAnsw;
};

#endif // !_CONFIG_