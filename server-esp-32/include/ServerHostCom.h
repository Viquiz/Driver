#ifndef _SERVER_HOST_COMMUNICATION_
#define _SERVER_HOST_COMMUNICATION_
#include <Arduino.h>
#include <ArduinoJson.h>

void registerClient(const uint8_t *macAddr);

#endif // !_SERVER_HOST_COMMUNICATION_