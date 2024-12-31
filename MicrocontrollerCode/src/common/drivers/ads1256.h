#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for ADS1256

#include <Arduino.h>
#include <ADS1256.h>
#include <SPI.h>
#include <array>
// #include "../math/vector.h"

namespace Cesium {

class Ads1256 {
private:
    ADS1256_Base device;
    std::array<float, 8> calibrate_slopes;
    std::array<float, 8> calibrate_intercepts;
    size_t channel_count;
    
    uint32_t last_read_ms = 0;

    uint8_t cs_pin;
    uint8_t drdy_pin;
    SPIClass* spi_instance_ptr;
    

public:

    // Ads1256();
    Ads1256(uint8_t cs_pin, uint8_t drdy_pin, SPIClass* spi_instance, size_t num_readings);
    bool set_calibration(float* slopes = nullptr, float* intercepts = nullptr);
    bool setup();
    bool read();

    std::array<float, 8> voltage_raw;
    std::array<float, 8> voltage_volt;



    static constexpr float VREF = 2.500;

    static int singleEndedChannels[8];
    static int differentialChannels[4];
};

int Ads1256::singleEndedChannels[] = {SING_0, SING_1, SING_2, SING_3, SING_4, SING_5, SING_6, SING_7}; //Array to store the single-ended channels
int Ads1256::differentialChannels[] = {DIFF_0_1, DIFF_2_3, DIFF_4_5, DIFF_6_7}; //Array to store the differential channels


} // namespace Cesium