#pragma once

#include "SensorBase.h"
#include "AccelerometerBase.h"
#include "GyroscopeBase.h"
#include "MagnetometerBase.h"

namespace Cesium {
namespace Sensor {


class MockSensorBase : public SensorBase {
public:
    virtual bool configure(const char* config_name) {return false;};
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
    virtual bool configure(const char* config_name) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        accel_mps2[0][0] += 1.0;
        accel_mps2[1][0] += 2.0;
        accel_mps2[2][0] += 3.0;

        accel_body_mps2[0][0] += 1.0;
        accel_body_mps2[1][0] += 2.0;
        accel_body_mps2[2][0] += 3.0;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;}
    uint32_t get_read_interval_ms() {return read_interval_ms;}

    const TwoWire* get__i2c_instance() {return _i2c_instance;} // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;} // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; } // For UART comms
    const Interfaces get_interface() {return interface;}

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;}

    Vector3<float> get_accel_mps2() {return accel_mps2;}
    Vector3<float> get_accel_body_mps2() {return accel_body_mps2;}
};


class MockGyroBase : public GyroscopeBase {
public:
    virtual bool configure(const char* config_name) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        w_dps[0][0] += 1.0;
        w_dps[1][0] += 2.0;
        w_dps[2][0] += 3.0;

        w_body_dps[0][0] += 1.0;
        w_body_dps[1][0] += 2.0;
        w_body_dps[2][0] += 3.0;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;}
    uint32_t get_read_interval_ms() {return read_interval_ms;}

    const TwoWire* get__i2c_instance() {return _i2c_instance;} // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;} // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; } // For UART comms
    const Interfaces get_interface() {return interface;}

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;}

    Vector3<float> get_w_dps() {return w_dps;}
    Vector3<float> get_w_body_dps() {return w_body_dps;}
};

class MockMagBase : public MagnetometerBase {

public:

    virtual bool configure(const char* config_name) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        B_uT[0][0] += 1.0;
        B_uT[1][0] += 2.0;
        B_uT[2][0] += 3.0;

        B_body_uT[0][0] += 1.0;
        B_body_uT[1][0] += 2.0;
        B_body_uT[2][0] += 3.0;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;};
    uint32_t get_read_interval_ms() {return read_interval_ms;};

    const TwoWire* get__i2c_instance() {return _i2c_instance;}; // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;}; // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; }; // For UART comms
    const Interfaces get_interface() {return interface;};

    Matrix3<float> get_body_to_sensor() {return body_to_sensor;};

    Vector3<float> get_B_uT() {return B_uT;};
    Vector3<float> get_B_body_uT() {return B_body_uT;};

};


} // namespace Sensor
} // namespace Cesium