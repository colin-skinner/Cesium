#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: 1.X.X
// PURPOSE: Driver code for BMI323

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "sensor_bases/AccelerometerBase.h"
#include "sensor_bases/GyroscopeBase.h"

#include "arduino_bmi323.h"
extern "C" {
#include "bosch_bmi323.h"
#include "bosch_bmi3_defs.h"
#include "bosch_bmi323_defs.h"
}

#define GRAVITY_EARTH  (9.80665f)

namespace Cesium {
namespace Sensor {

class Bmi323 : public AccelerometerBase, public GyroscopeBase {

private:
    // BMI323 lower-level driver structs
    struct bmi3_dev dev;
    struct bmi3_sensor_data sensor_data;
    struct bmi3_sens_config config;

    // Interrupts
    uint16_t interrupt_status;

    uint8_t _cs_pin;
    SPIClass* spi_instance;
    SPISettings settings;



    bool zero_structs();
public:
    Bmi323();

    Bmi323(TwoWire* i2c_instance);

    Bmi323(uint8_t cs_pin, SPIClass* spi_instance);
    
    bool configure(const char* config_name);
    bool setup();
    bool read();

    // Helper functions
    bool readAccelerometer(float& x, float& y, float& z);
    bool readGyroscope(float& x, float& y, float& z);

    // Overloaded to use with contiguous memory locations
    bool readAccelerometer(float* buff);
    bool readGyroscope(float* buff);

    bool readReg(uint8_t reg, uint16_t* buffer, size_t len);
    bool writeReg(uint8_t reg, const uint16_t *buffer, size_t len);
    bool read_chip_id();

    float lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width);
    float lsb_to_dps(int16_t val, float dps, uint8_t bit_width);

};



}
}