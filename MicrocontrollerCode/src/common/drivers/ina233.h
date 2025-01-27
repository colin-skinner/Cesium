#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for INA233

#include <Arduino.h>
#include <infinityPV_INA233.h>


namespace Cesium {
namespace Sensor {

class Ina233 {
private:
    INA233 device;
    uint64_t last_read_us = 0;

public:

    Ina233(uint8_t ina_addr);
    bool setup();
    bool read();

    uint8_t addr = 0x40;
    float bus_current_mA = 0;
    float bus_voltage_V = 0;


    int lastRead = 0;
    float R_shunt_IC1=0.01;   //call RS2 on the board 
    float I_max_IC1=2;

    //variables to catch the outputs from setCalibration()
    uint16_t CAL=0;
    int16_t m_c=0;
    int16_t m_p=0;
    int8_t R_c=0;
    int8_t R_p=0;
    uint8_t Set_ERROR=0;
    float Current_LSB=0;
    float Power_LSB=0;
    //variable to check the loaded calibration
    uint16_t Read_CAL=0;

};

} // namespace Sensor
} // namespace Cesium