#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for Adafruit ICM20948

#include <Arduino.h>
#include <Adafruit_ICM20948.h>
#include <SPI.h>
#include "../math/vector.h"

#include "sensor_bases/AccelerometerBase.h"
#include "sensor_bases/GyroscopeBase.h"
#include "sensor_bases/MagnetometerBase.h"

namespace Cesium {
namespace Sensor {

class Icm20948 : public AccelerometerBase, public GyroscopeBase, public MagnetometerBase {
private:
    Adafruit_ICM20948 device;

    // TODO: add calibration stuff
    Vector3<float> accel_factor[3];
    Vector3<float> gyro_factor[3];
    Vector3<float> mag_factor[3];

    uint8_t cs_pin;

    // To make them class-wide
    sensors_event_t accel_event;
    sensors_event_t gyro_event;
    sensors_event_t temp_event;
    sensors_event_t mag_event;
    

public:

    Icm20948();
    Icm20948(uint8_t cs_pin, SPIClass* spi_instance);
    
    bool configure(const char* config_name);
    bool setup();
    bool read();

};



} // namespace Sensor
} // namespace Cesium