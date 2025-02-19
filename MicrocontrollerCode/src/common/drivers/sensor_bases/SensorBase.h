#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "../../globals.h"
#include "../../math/quaternion.h"

namespace Cesium {
namespace Sensor {


enum class Interfaces {
    SPI,
    I2C,
    Serial,
    NOT_SET
};

class SensorBase {

protected:
    uint32_t last_read_time_ms;
    uint32_t read_interval_ms;

    TwoWire* _i2c_instance; // For I2C comms
    SPIClass* _spi_instance; // For SPI comms
    HardwareSerial* _serial_instance; // For UART comms
    Interfaces interface;

    Quaternion<float> body_to_sensor;

    // TODO: add temperature
    float temp_C;
    
public:
    SensorBase();
    virtual ~SensorBase() = 0;

    void attach_I2C(TwoWire* I2C);   
    void attach_SPI(SPIClass* spi);
    void attach_serial(HardwareSerial* serial);


    virtual bool configure(const char* config_name) = 0;
    virtual bool setup() = 0;
    virtual bool read(); 
    virtual void attach_int_pin(uint8_t pin, void (*func)(), uint8_t mode = ONHIGH);

    DELETE_COPY_AND_ASSIGNMENT(SensorBase)

    void set_rotation_matrix(Matrix3<float> matrix) {body_to_sensor = quat_from_R(matrix);}
    uint32_t get_interval_ms() const {return read_interval_ms;}
    uint32_t get_last_read_time_ms() const {return last_read_time_ms;}
    Interfaces get_interface() const {return interface;}
    inline float get_temp_C() const {return temp_C;}
};

} // namespace Sensor
} // namespace Cesium