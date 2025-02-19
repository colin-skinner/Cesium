#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for UBloxGps GPS breakout

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

#include "sensor_bases/GpsBase.h"

namespace Cesium {
namespace Sensor {
class UBloxGps : public GpsBase {
private:
    SFE_UBLOX_GNSS device;
    uint64_t last_read_us = 0;

    uint8_t cs_pin;
    SPIClass* spi_instance;
    uint32_t clock_freq;
    uint8_t i2c_addr;

public:

    UBloxGps(uint8_t cs_pin, SPIClass* spi_instance);
    UBloxGps(TwoWire* i2c_instance, uint8_t address = 0x42);
    UBloxGps(HardwareSerial* serial_instance); // Assumes correct baud rate is already specfied
    bool setup();
    bool read();

    inline uint32_t get_latitude_scaled() {return latitude_scaled;}
    inline uint32_t get_longitude_scaled() {return longitude_scaled;}
    inline float get_latitude_deg() {return latitude_deg;}
    inline float get_longitude_deg() {return longitude_deg;}
    inline float get_altitude_m() {return altitude_m;}
    inline uint8_t get_satellites_in_view() {return satellites_in_view;}
    inline uint32_t get_heading_scaled() {return heading_scaled;}
    inline uint32_t get_ground_speed_mmps() {return ground_speed_mmps;}
};

} // namespace Sensor
} // namespace Cesium