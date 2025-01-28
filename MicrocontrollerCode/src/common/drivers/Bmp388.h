#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for BMP 388

#include <Arduino.h>
#include <Adafruit_BMP3XX.h>
#include <SPI.h>
#include "../math/vector.h"

#include "sensor_bases/BarometerBase.h"

namespace Cesium {
namespace Sensor {

class Bmp388 : public BarometerBase {
private:
    Adafruit_BMP3XX device;

    uint8_t cs_pin;

    float _sea_level_pressure_hPa = 1016.30;

public:

    Bmp388();
    Bmp388(uint8_t cs_pin, SPIClass* spi_instance);
    
    void set_sea_level_pressure(float sea_level_pressure_hPa);
    bool configure(const char* config_name);
    bool setup();
    bool read();

};



} // namespace Sensor
} // namespace Cesium