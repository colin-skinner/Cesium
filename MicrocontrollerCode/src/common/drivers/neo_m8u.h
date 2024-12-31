#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for NeoM8U GPS breakout

#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
namespace Cesium {

class NeoM8U {
private:
    SFE_UBLOX_GNSS device;
    uint64_t last_read_us = 0;

    uint8_t cs_pin;
    SPIClass* spi_instance;
    uint32_t spi_freq;

public:

    NeoM8U(uint8_t cs_pin, SPIClass* spi_instance);
    bool setup();
    bool read();

    uint32_t latitude = 0;
    uint32_t longitude = 0;
    uint32_t altitude_m = 0;
    uint8_t satellites_in_view;
};

} // namespace Cesium