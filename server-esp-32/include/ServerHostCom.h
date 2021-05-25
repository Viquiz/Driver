#ifndef _SERVER_HOST_COMMUNICATION_
#define _SERVER_HOST_COMMUNICATION_
#include <Arduino.h>
#include <ArduinoJson.h>
// ----------------------------------------
// Using JSON for communication between server (ESP-32) and host (PC, Laptop)
// ----------------------------------------
enum message_t : uint8_t
{
    // Forward client MAC address to host for processing
    // @param "mac": Array
    REG_CLIENT,
    // Host send to server, server will notify the "id" to client
    // @param "id": Integer,
    // @param "mac": Array
    RESPOND_REG_CLIENT,
    // Forward client answer to host
    // @param "mac": Array, 
    // @param "btn": Integer
    CLIENT_ANSWER,
};

void registerClient(const uint8_t *macAddr);

#endif // !_SERVER_HOST_COMMUNICATION_