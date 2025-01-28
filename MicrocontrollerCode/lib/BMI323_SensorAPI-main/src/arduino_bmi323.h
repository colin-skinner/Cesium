#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

extern "C" {
    /*
    Things used from here:
    - BMI323_CHIP_ID
    */
    #include "bosch_bmi323_defs.h"

    /*
    Things used from here:
    - 
    */
    #include "bosch_bmi3_defs.h"
}

#define GRAVITY_EARTH  (9.80665f)

class BMI323 {
// TODO: Implement I2C
private:
    uint8_t _cs_pin;
    SPIClass* _spi_instance;

    uint8_t chip_id;

    // Define SPI settings: 1 MHz, MSB first, Mode 0
    SPISettings _spi_settings;

public:
    // BMI323();
    // BMI323(TwoWire* i2c_instance); 

    BMI323(uint8_t cs_pin, SPIClass* spi_instance = &SPI);

    uint8_t read_chip_id();

    // Initializes according to datasheet diagrams pp. 14-19
    int8_t initialize(); // TODO: figure out registers we want to change

    bool read_reg(uint8_t reg, uint16_t* rx_buffer, size_t size);
    bool write_reg(uint8_t reg, uint16_t* tx_buffer, size_t size);

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