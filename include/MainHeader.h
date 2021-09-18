#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define _TASK_STATUS_REQUEST
#include "TaskSchedulerDeclarations.h"

#define ENABLE_LOGGING
#include "SerialLog.h"

#include "ServerHostProtocol.h"
#include "ServerClientProtocol/ServerClientProtocol.h"

#define CHANNEL 0
#define SERIAL_BUFFER_SIZE 256