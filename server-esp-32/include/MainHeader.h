#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define _TASK_STATUS_REQUEST
#include "TaskSchedulerDeclarations.h"

#define LOGGING
#include "SerialLog.h"

#define CHANNEL 0