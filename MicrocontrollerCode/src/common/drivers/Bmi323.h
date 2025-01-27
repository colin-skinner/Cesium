#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

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

class Bmi323 : public GyroscopeBase {

private:
    // BMI323 lower-level driver structs
    struct bmi3_dev dev;
    struct bmi3_sensor_data sensor_data;
    struct bmi3_sens_config config;

    // Interrupts
    uint16_t interrupt_status;

    uint8_t _cs_pin;
    SPIClass* spi_instance;

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

    /*!
    * @brief This API prints the execution status
    */
    void bmi3_error_codes_print_result(const char api_name[], int8_t rslt);

    /*!
    *  @brief This internal API is used to set configurations for accel.
    *
    *  @param[in] dev       : Structure instance of bmi3_dev.
    *
    *  @return Status of execution.
    */
    static int8_t set_accel_config(struct bmi3_dev *dev);

    /*!
    *  @brief This internal function converts lsb to meter per second squared for 16 bit accelerometer for
    *  range 2G, 4G, 8G or 16G.
    *
    *  @param[in] val       : LSB from each axis.
    *  @param[in] g_range   : Gravity range.
    *  @param[in] bit_width : Resolution for accel.
    *
    *  @return Accel values in meter per second squared.
    */
    static float lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width = BMI3_16_BIT_RESOLUTION);

    /*!
    *  @brief This function converts lsb to degree per second for 16 bit gyro at
    *  range 125, 250, 500, 1000 or 2000dps.
    *
    *  @param[in] val       : LSB from each axis.
    *  @param[in] dps       : Degree per second.
    *  @param[in] bit_width : Resolution for gyro.
    *
    *  @return Degree per second.
    */
    static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width = BMI3_16_BIT_RESOLUTION);


};



}
}