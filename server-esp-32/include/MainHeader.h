#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define _TASK_STATUS_REQUEST
#include "TaskSchedulerDeclarations.h"

#define LOGGING
#include "SerialLog.h"

#include "Packet.h"

#define CHANNEL 0
#define PASSWORD_LEN 4
#define BEACON_INTERVAL (1000 * TASK_MILLISECOND)