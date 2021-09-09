// A thin wrapper for ArduinoLog
#ifndef _SERIAL_LOG_
#define _SERIAL_LOG_

#if defined(LOGGING)
#define LOGGER Serial
#elif defined(LOGGING_BT)
#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
#define LOGGER SerialBT
#endif

#if defined(LOGGING) || defined(LOGGING_BT)

#include "ArduinoLog.h"
extern Logging Log;
#define LOG_INIT(lv)            Log.begin   (lv, &LOGGER)
// ## __VA_ARGS__ is not portable

#define LOG_FATAL(msg, ...)     Log.fatal   (msg CR, ## __VA_ARGS__)
#define LOG_ERROR(msg, ...)     Log.error   (msg CR, ## __VA_ARGS__)
#define LOG_WARNING(msg, ...)   Log.warning (msg CR, ## __VA_ARGS__)
#define LOG_NOTICE(msg, ...)    Log.notice  (msg CR, ## __VA_ARGS__)
#define LOG_TRACE(msg, ...)     Log.trace   (msg CR, ## __VA_ARGS__)
#define LOG_VERBOSE(msg, ...)   Log.verbose (msg CR, ## __VA_ARGS__)

#else

#define LOG_INIT(lv)

#define LOG_FATAL(msg)
#define LOG_ERROR(msg)
#define LOG_WARNING(msg)
#define LOG_NOTICE(msg)
#define LOG_TRACE(msg)
#define LOG_VERBOSE(msg)

#endif

#endif // _SERIAL_LOG_