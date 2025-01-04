#pragma once

#include <TimeLib.h>
#include <Arduino.h>
#include <time.h>
#include <DS3232RTC.h>

#include <Wire.h>

#define MILLISECS_PER_DAY  ((time_t)(SECS_PER_DAY * 1000UL)) 

namespace Cesium {
class Clock {

public:
    static uint32_t millis_since_midnight;
    static uint32_t millis_last_time_sync;
    tmElements_t midnight_time;

    static bool second_accuracy;
    static bool millisecond_accuracy;

    // DAY, MONTH, and YEAR ALL START AT 1
    static bool jump_clock(uint32_t milliseconds_since_midnight, uint8_t day, uint8_t month, uint16_t yr);

    static uint8_t jump_clock_RTC(DS3232RTC& rtc);

    static uint32_t get_millis_since_midnight();

};
};

