#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for MS5611/MS5607

#include <Arduino.h>
#include <MS5611_SPI.h>
#include <SPI.h>
namespace Cesium {

class Ms5611 {
private:
    MS5611_SPI device{(int)0, (SPIClass*)0}; // Redefining in Ms5611() with actual values
    uint32_t last_read_us = 0;

public:


    Ms5611(uint8_t cs_pin, SPIClass* spi_instance);
    bool setup();
    bool read();

    float SEALEVELPRESSURE_HPA = 1018.90; // Sea level pressure of LA 

    float temp, pressure, altitude;

    const float R = 287.052; // specific gas constant J/kg/K 
    const float g = 9.80665; // standard gravity m/s^2
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude (15 degC)
};

} // namespace Cesium