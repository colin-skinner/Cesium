#include "clock.h"

namespace Cesium {

uint32_t Clock::millis_since_midnight = 0;
uint32_t Clock::millis_last_time_sync = 0;

bool Clock::second_accuracy = false;
bool Clock::millisecond_accuracy = false;

bool Clock::jump_clock(uint32_t milliseconds_since_midnight, uint8_t day, uint8_t month, uint16_t yr)
{
    millis_since_midnight = milliseconds_since_midnight;

    if (millis_since_midnight >= MILLISECS_PER_DAY) {
        return false;
    }

    millis_last_time_sync = millis();

    second_accuracy = true;
    millisecond_accuracy = true;

    return true;
}

uint8_t Clock::jump_clock_RTC(DS3232RTC &rtc)
{
    tmElements_t tm;
    uint8_t err_code = DS3232RTC::read(tm);

    millis_since_midnight = (tm.Second + SECS_PER_MIN * tm.Minute + SECS_PER_HOUR * tm.Hour) * 1000;

    if (err_code != ESP_OK) {
        return err_code;
    }

    jump_clock(millis_since_midnight, tm.Day, tm.Month, tm.Year);

    second_accuracy = true;
    millisecond_accuracy = false;

    return ESP_OK;
}

uint32_t Clock::get_millis_since_midnight()
{
    if (millis_since_midnight >= millis_last_time_sync) {
        uint32_t midnight_to_program_start_ms = millis_since_midnight - millis_last_time_sync;
        return midnight_to_program_start_ms + millis();
    }

    return millis_last_time_sync % MILLISECS_PER_DAY;
}

}