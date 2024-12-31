#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for Adafruit ICM20948

#include <Arduino.h>
#include <Adafruit_ICM20948.h>
#include <SPI.h>
#include "../math/vector.h"

namespace Cesium {

class Icm20948 {
private:
    Adafruit_ICM20948 device;
    float offset[3];
    uint32_t last_read_ms = 0;

    uint8_t cs_pin;
    SPIClass* spi_instance_ptr;
    

public:

    Icm20948();
    Icm20948(uint8_t cs_pin, SPIClass* spi_instance);
    bool setup();
    bool read();

    float temp;
    // Vector3<float> accel;
    // Vector3<float> gyro;
    // Vector3<float> B_field;
    

    sensors_vec_t accel;
    sensors_vec_t gyro;
    sensors_vec_t B_field;
};

} // namespace Cesium