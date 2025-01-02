#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "../globals.h"

namespace Cesium {

// Timestamp can be set (using RTCC or WiFi) and read
struct Timestamp {

    tm timeinfo;

    // Must call Network.begin() before calling this
    // --> gmtOffset_sec = GMT_offset_hours * 3600
    // --> e.g. GMT-8 = -8.00 * 3600 = 28'800
    bool from_WiFi(const long gmtOffset_sec = 0, bool daylight_savings = false) {
        
        uint32_t daylightOffset_sec = 0;

        if (daylight_savings)
            daylightOffset_sec = 3600;
        
        const char* ntpServer = "pool.ntp.org";

        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    }

    // Loads into timeinfo variable
    bool get_time() {

        if(!getLocalTime(&timeinfo)){
            DEBUGLN("Failed to obtain time");
            return false;
        }
        return true;
    }

    bool to_bytes() {
        get_time();

        
    }

    bool printLocalTime(){
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return false;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        Serial.print("Day of week: ");
        Serial.println(&timeinfo, "%A");
        Serial.print("Month: ");
        Serial.println(&timeinfo, "%B");
        Serial.print("Day of Month: ");
        Serial.println(&timeinfo, "%d");
        Serial.print("Year: ");
        Serial.println(&timeinfo, "%Y");
        Serial.print("Hour: ");
        Serial.println(&timeinfo, "%H");
        Serial.print("Hour (12 hour format): ");
        Serial.println(&timeinfo, "%I");
        Serial.print("Minute: ");
        Serial.println(&timeinfo, "%M");
        Serial.print("Second: ");
        Serial.println(&timeinfo, "%S");

        Serial.println("Time variables");
        char timeHour[3];
        strftime(timeHour,3, "%H", &timeinfo);
        Serial.println(timeHour);
        char timeWeekDay[10];
        strftime(timeWeekDay,10, "%A", &timeinfo);
        Serial.println(timeWeekDay);
        Serial.println();

        return true;
    }


    
};

Timestamp time;

    
}