// A thin wrapper for ArduinoLog
#ifndef _SERIAL_LOG_
#define _SERIAL_LOG_

#if defined(ENABLE_LOGGING)
#define LOGGER Serial

#elif defined(ENABLE_BLUETOOTH_LOGGING)
#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
#define LOGGER SerialBT
#endif

#if !defined(ENABLE_LOGGING) && !defined(ENABLE_BLUETOOTH_LOGGING)
#define DISABLE_LOGGING
#endif

#include "ArduinoLog.h"
extern Logging Log;

#endif // !_SERIAL_LOG_