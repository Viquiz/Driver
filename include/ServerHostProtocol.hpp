#ifndef _SERVER_HOST_PROTOCOL_
#define _SERVER_HOST_PROTOCOL_
#include <Arduino.h>
// ----------------------------------------
// Communication between server (ESP-32) and host (PC, Laptop)
// ----------------------------------------
enum message_t : uint8_t
{
    // The JSON contains no "type" object or unknown type, this treats as an error
    INVALID_TYPE = 0,
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

#endif // !_SERVER_HOST_PROTOCOL_