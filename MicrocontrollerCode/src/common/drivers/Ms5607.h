#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: 1.X.X
// PURPOSE: Driver code for MS5611/MS5607

#include <Arduino.h>
#include <MS5611_SPI.h>
#include <SPI.h>

#include "sensor_bases/BarometerBase.h"

namespace Cesium {
namespace Sensor {
class Ms5607 : public BarometerBase {
private:
    MS5611_SPI device{(int)0, (SPIClass*)0}; // Redefining in Ms5607() with actual values

    uint8_t _cs_pin;

    float P_offsets[2]; // Linear fit (a,b, in ax + b)

    // Constants according to the datasheet
    const float R = 287.052; // specific gas constant J/kg/K 
    const float g = 9.80665; // standard gravity m/s^2
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude (15 degC)

public:


    Ms5607(uint8_t cs_pin, SPIClass* spi_instance);
    virtual ~Ms5607();
    bool configure(const char* config_name);
    bool calibrate(float current_alt_m = 0);
    void set_P_offsets(float a, float b);
    bool setup();
    bool read();

    float SEALEVELPRESSURE_KPA = 101.89; // Sea level pressure of LA 



    float get_temp_C() {return temp_C;}
    float get_pressure_kPa() {return pressure_kPa;}
    float get_altitude_m() {return altitude_m;}
};


} // namespace Sensor
} // namespace Cesium