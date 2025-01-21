#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "../../globals.h"

namespace Cesium {
namespace Sensor {
class SensorBase {

protected:
    uint32_t last_read_time_ms;
    uint32_t read_interval_ms;

    TwoWire* _i2c_instance; // For I2C comms
    SPIClass* _spi_instance; // For SPI comms
    HardwareSerial* _serial_instance; // For UART comms
public:
    SensorBase();
    virtual ~SensorBase() = 0;
    virtual bool configure(const char* config_filename) = 0;
    virtual bool setup() = 0;
    virtual bool read(); 
    virtual void attach_int_pin(uint8_t pin, void (*func)(), bool mode = ONHIGH);

    DELETE_COPY_AND_ASSIGNMENT(SensorBase)

    uint32_t get_interval_ms() const {return read_interval_ms;}
    uint32_t get_last_read_time_ms() const {return last_read_time_ms;}
};

} // namespace Sensor
} // namespace Cesium