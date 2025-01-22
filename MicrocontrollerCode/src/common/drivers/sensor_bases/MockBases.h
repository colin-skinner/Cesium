#pragma once

#include "SensorBase.h"
#include "AccelerometerBase.h"
#include "GyroscopeBase.h"
#include "MagnetometerBase.h"

namespace Cesium {
namespace Sensor {


class MockSensorBase : public SensorBase {
public:
    virtual bool configure(const char* relative_filename) {return false;};
    virtual bool setup() {return false;};

    uint32_t get_last_read_time_ms() {return last_read_time_ms;};
    uint32_t get_read_interval_ms() {return read_interval_ms;};

    const TwoWire* get__i2c_instance() {return _i2c_instance;}; // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;}; // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; }; // For UART comms
    const Interfaces get_interface() {return interface;};

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;};
};


class MockAccelBase : public AccelerometerBase {

public:
    virtual bool configure(const char* relative_filename) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        accel_mps[0][0] += 1.0;
        accel_mps[1][0] += 2.0;
        accel_mps[2][0] += 3.0;

        accel_body_mps[0][0] += 1.0;
        accel_body_mps[1][0] += 2.0;
        accel_body_mps[2][0] += 3.0;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;};
    uint32_t get_read_interval_ms() {return read_interval_ms;};

    const TwoWire* get__i2c_instance() {return _i2c_instance;}; // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;}; // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; }; // For UART comms
    const Interfaces get_interface() {return interface;};

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;};

    Vector3<float> get_accel_mps() {return accel_mps;};
    Vector3<float> get_accel_body_mps() {return accel_body_mps;};
};


class MockGyroBase : public GyroscopeBase {
public:
    virtual bool configure(const char* relative_filename) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        w_rps[0][0] += 1.0;
        w_rps[1][0] += 2.0;
        w_rps[2][0] += 3.0;

        w_body_rps[0][0] += 1.0;
        w_body_rps[1][0] += 2.0;
        w_body_rps[2][0] += 3.0;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;};
    uint32_t get_read_interval_ms() {return read_interval_ms;};

    const TwoWire* get__i2c_instance() {return _i2c_instance;}; // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;}; // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; }; // For UART comms
    const Interfaces get_interface() {return interface;};

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;};

    Vector3<float> get_w_rps() {return w_rps;};
    Vector3<float> get_w_body_rps() {return w_body_rps;};
};



} // namespace Sensor
} // namespace Cesium