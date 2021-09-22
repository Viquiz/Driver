// A thin wrapper for ArduinoLog
#ifndef _SERIAL_LOG_
#define _SERIAL_LOG_

#ifndef DISABLE_LOGGING
#ifdef BLUETOOTH_LOGGING
#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
#define LOGGER SerialBT
#else
#define LOGGER Serial
#endif
#include <ArduinoLog.h>
extern Logging Log;
#endif

#endif // !_SERIAL_LOG_