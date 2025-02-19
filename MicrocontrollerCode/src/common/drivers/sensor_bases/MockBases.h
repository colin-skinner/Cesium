#pragma once

#include "SensorBase.h"
#include "AccelerometerBase.h"
#include "GyroscopeBase.h"
#include "MagnetometerBase.h"
#include "GpsBase.h"

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

    Quaternion<float> get_body_to_sensor() {return body_to_sensor;};
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

    Quaternion<float> get_body_to_sensor() {return body_to_sensor;}

    Vector3<float> get_accel_mps2() {return accel_mps2;}
    Vector3<float> get_accel_body_mps2() {return accel_body_mps2;}
};


class MockGyroBase : public GyroscopeBase {
public:
    virtual bool configure(const char* config_name) {return false;};
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

    uint32_t get_last_read_time_ms() {return last_read_time_ms;}
    uint32_t get_read_interval_ms() {return read_interval_ms;}

    const TwoWire* get__i2c_instance() {return _i2c_instance;} // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;} // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; } // For UART comms
    const Interfaces get_interface() {return interface;}

    Quaternion<float> get_body_to_sensor() {return body_to_sensor;}

    Vector3<float> get_w_rps() {return w_rps;}
    Vector3<float> get_w_body_rps() {return w_body_rps;}
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

    Quaternion<float> get_body_to_sensor() {return body_to_sensor;};

    Vector3<float> get_B_uT() {return B_uT;};
    Vector3<float> get_B_body_uT() {return B_body_uT;};

};

class MockGpsBase : public GpsBase {

public:

    virtual bool configure(const char* config_name) {return false;};
    virtual bool setup() {return false;};
    virtual bool read() {

        SensorBase::read();

        // Arbitrary
        latitude_scaled = 420;
        longitude_scaled = 42000;
        latitude_deg = 12.1;
        longitude_deg = 12.2;
        altitude_m = 10000.4;
        satellites_in_view = 12;
        heading_scaled = 1.1;
        ground_speed_mmps = 18;

        return true;
    }

    uint32_t get_last_read_time_ms() {return last_read_time_ms;};
    uint32_t get_read_interval_ms() {return read_interval_ms;};

    const TwoWire* get__i2c_instance() {return _i2c_instance;}; // For I2C comms
    const SPIClass* get__spi_instance() {return _spi_instance;}; // For SPI comms
    const HardwareSerial* get__serial_instance() {return _serial_instance; }; // For UART comms
    const Interfaces get_interface() {return interface;};

    inline uint32_t get_latitude_scaled() {return latitude_scaled;}
    inline uint32_t get_longitude_scaled() {return longitude_scaled;}
    inline float get_latitude_deg() {return latitude_deg;}
    inline float get_longitude_deg() {return longitude_deg;}
    inline float get_altitude_m() {return altitude_m;}
    inline uint8_t get_satellites_in_view() {return satellites_in_view;}
    inline uint32_t get_heading_scaled() {return heading_scaled;}
    inline uint32_t get_ground_speed_mmps() {return ground_speed_mmps;}

};


} // namespace Sensor
} // namespace Cesium