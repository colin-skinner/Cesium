#pragma once

#include <Arduino.h>

#ifdef DEBUG_MODE
#define DEBUG(val) Serial.print(val)
#define DEBUG_FLUSH() Serial.flush()
#define DEBUGLN(val) Serial.println(val)
#else
#define DEBUG(val)
#define DEBUG_FLUSH()
#define DEBUGLN(val)
#endif

