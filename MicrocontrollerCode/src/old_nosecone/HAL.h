#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>

#include "../common/globals.h"
#include "../common/os/filesystem.h"
#include "../common/comms/CanBus.h"

#include "../common/drivers/Icm20948.h"
#include "../common/drivers/Ms5607.h"
#include "../common/drivers/Bmi323.h"
#include "../common/drivers/Bmp388.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Variables                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cesium {

    ////////////////////////////////////////////////////////////
    //                       Pinouts                          //
    ////////////////////////////////////////////////////////////
    // VSPI
    const uint8_t VSCK = 18;
    const uint8_t VMISO = 19;
    const uint8_t VMOSI = 23;

    // HSPI
    const uint8_t HSCK = 14;
    const uint8_t HMISO = 36;
    const uint8_t HMOSI = 13;

    // I2C
    // const uint8_t INA_ADDR = 0x40;
    // const uint8_t INA_SDA = 21;
    // const uint8_t INA_SCL = 22;

    // Serial 
    // const uint8_t GPS_RX = 16;
    // const uint8_t GPS_TX = 17;
    // const uint8_t GPS_I2C_PORT = 2; //This is not a pin
    // inline HardwareSerial gps_serial(GPS_I2C_PORT);

    /* ---------- Pinouts ---------- */
    // Transmitter
    // const uint8_t TRANSMITTER_RST = 27;
    // const uint8_t TRANSMITTER_CS = 5;
    // const uint8_t TRANSMITTER_D0 = 2;
    // const uint8_t TRANSMITTER_D1 = 4;
    // const uint8_t TRANSMITTER_TX_EN = 25;
    // const uint8_t TRANSMITTER_RX_EN = 26;

    // Altimeter 1 (BMP390)
    const uint8_t ALTIMETER1_CS = 32;

    // Altimeter 2 (MS5607_CS)
    const uint8_t ALTIMETER2_CS = 2;

    // IMU 1 (BMI323)
    const uint8_t IMU1_CS = 16;

    // IMU 2 (ICM20948)
    const uint8_t IMU2_CS = 33;

    // SD/Flash
    const uint8_t SD_CS = 32;

    const uint8_t LED = 2;

    // CAN
    const uint8_t CAN_RX = 25;
    const uint8_t CAN_TX = 26;


    ////////////////////////////////////////////////////////////
    //                     Interfaces                         //
    ////////////////////////////////////////////////////////////

    // Interfaces
    extern SPIClass hspi;
    extern SPIClass vspi;
    extern CanBus can_bus;



    ////////////////////////////////////////////////////////////
    //                       Objects                          //
    ////////////////////////////////////////////////////////////

    // Objects
    extern FileSystem filesystem;
    extern Sensor::Bmp388 altimeter1;
    extern Sensor::Ms5607 altimeter2;
    extern Sensor::Icm20948 imu2;
    extern Sensor::Bmi323 imu1;

    ////////////////////////////////////////////////////////////
    //                     Functions                          //
    ////////////////////////////////////////////////////////////

    void init_cs_pins();

    void init_interfaces();

    void init_sensors();
    
}
