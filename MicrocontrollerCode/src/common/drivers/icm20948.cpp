#include "Icm20948.h"
#include "../globals.h"
#include "../../config/ConfigReader.h"
#include <ArduinoJson.h>

namespace Cesium {
namespace Sensor {
Icm20948::Icm20948()
    : accel_bias{}
    , gyro_bias{}
    , mag_bias{}
    , cs_pin{0}
{
    temp_C = 0;
    _spi_instance = &SPI;
}

Icm20948::Icm20948(uint8_t cs_pin, SPIClass* spi_instance)
{
    _spi_instance = spi_instance;
    interface = Interfaces::SPI;
    this->cs_pin = cs_pin;
}

bool Icm20948::configure(const char* config_name)
{
    JsonDocument config;
    File::json_open(config_name, config);

    RETURN_FALSE_IF_FALSE(File::json_extract(config, "body_to_sensor", body_to_sensor));

    RETURN_FALSE_IF_FALSE(File::json_extract(config, "acceleration_bias", accel_bias));
    RETURN_FALSE_IF_FALSE(File::json_extract(config, "gyroscope_bias", gyro_bias));
    RETURN_FALSE_IF_FALSE(File::json_extract(config, "magnetometer_bias", mag_bias));
    
    return false;
}

bool Icm20948::setup()
{
    bool setup = false;
    for (int i=0; i<3000; i++) {
        if(device.begin_SPI(cs_pin, _spi_instance)) {
            DEBUGLN("found!");
            setup = true;
            break;
        }
        DEBUG(".");
        
    }

    if (!setup) {
        DEBUGLN("Failed to find imu20948 chip");
        return false;
    }

    
    // imu.setAccelRange(imu20948_ACCEL_RANGE_16_G);
    DEBUG("Accelerometer range set to: ");
    switch (device.getAccelRange()) {
    case ICM20948_ACCEL_RANGE_2_G:
        DEBUGLN("+-2G");
        break;
    case ICM20948_ACCEL_RANGE_4_G:
        DEBUGLN("+-4G");
        break;
    case ICM20948_ACCEL_RANGE_8_G:
        DEBUGLN("+-8G");
        break;
    case ICM20948_ACCEL_RANGE_16_G:
        DEBUGLN("+-16G");
        break;
    }
    DEBUGLN("OK");

    // imu.setGyroRange(imu20948_GYRO_RANGE_2000_DPS);
    DEBUG("Gyro range set to: ");
    switch (device.getGyroRange()) {
    case ICM20948_GYRO_RANGE_250_DPS:
        DEBUGLN("250 degrees/s");
        break;
    case ICM20948_GYRO_RANGE_500_DPS:
        DEBUGLN("500 degrees/s");
        break;
    case ICM20948_GYRO_RANGE_1000_DPS:
        DEBUGLN("1000 degrees/s");
        break;
    case ICM20948_GYRO_RANGE_2000_DPS:
        DEBUGLN("2000 degrees/s");
        break;
    }

    // //  imu.setAccelRateDivisor(4095);
    uint16_t accel_divisor = device.getAccelRateDivisor();
    float accel_rate = 1125 / (1.0 + accel_divisor);

    DEBUG("Accelerometer data rate divisor set to: ");
    DEBUGLN(accel_divisor);
    DEBUG("Accelerometer data rate (Hz) is approximately: ");
    DEBUGLN(accel_rate);

    //  imu.setGyroRateDivisor(255);
    uint8_t gyro_divisor = device.getGyroRateDivisor();
    float gyro_rate = 1100 / (1.0 + gyro_divisor);

    DEBUG("Gyro data rate divisor set to: ");
    DEBUGLN(gyro_divisor);
    DEBUG("Gyro data rate (Hz) is approximately: ");
    DEBUGLN(gyro_rate);

    // Setup magnetometer
    // device.writeExternalRegister(ICM20X_B3_I2C_SLV0_ADDR, )
    // Serial.println(device.enableI2CMaster(true));
    // Serial.println(device.readExternalRegister(AK09916_I2C_ADDR, AK09916_WIA2));
    
    // Serial.println("Writing: " + String(device.writeExternalRegister(AK09916_I2C_ADDR, AK09916_CNTL2, 0x00))); // power down
    // delay(100);

    // Serial.println("Writing: " + String(device.writeExternalRegister(AK09916_I2C_ADDR, AK09916_ST2, 0x01))); // power down
    // delay(100);





    // Serial.println("Writing: " + String(device.writeExternalRegister(AK09916_I2C_ADDR, AK09916_CNTL2, 0x04))); // Start conversion
    // Serial.println("Reading: " + String(device.readExternalRegister(AK09916_I2C_ADDR, AK09916_CNTL2))); // Start conversion
    
    return true;
}

bool Icm20948::read()
{
    SensorBase::read();

    /* Get a new normalized sensor event */
    device.getAccelerometerSensor()->getEvent(&accel_event);
    device.getGyroSensor()->getEvent(&gyro_event);
    device.getTemperatureSensor()->getEvent(&temp_event);
    // device.getMagnetometerSensor()->getEvent(&mag_event);

    // uint8_t status1 = device.readExternalRegister(AK09916_I2C_ADDR, 0x10); // Status 1
    // uint8_t status2 = device.readExternalRegister(AK09916_I2C_ADDR, 0x18); // Status 2
    // uint8_t ctrl2 = device.readExternalRegister(AK09916_I2C_ADDR, AK09916_CNTL2); // Status 2

    // Serial.print("Magnetometer Status 1: 0x");
    // Serial.println(status1, HEX);
    // Serial.print("Magnetometer Status 2: 0x");
    // Serial.println(status2, HEX);
    // Serial.print("Magnetometer Control 2: 0x");
    // Serial.println(ctrl2, HEX);
    // Serial.print(status1, HEX);
    // uint8_t buffer[6];

    // // Read 6 bytes of magnetometer data
    // for (int i = 0; i < 6; i++) {
    //     buffer[i] = device.readExternalRegister(AK09916_I2C_ADDR, AK09916_HXL + i);
    // }
    // for (int i = 0; i < 6; i++) {
    //     Serial.print(" " + String(buffer[i]));
    // }
    // Serial.println();
    


    // device.getEvent(&accel_event, &gyro_event, &temp_event, &mag_event);

    // If this doesn't work, just put "12" for the size
    memcpy(  &accel_mps2,  &accel_event.acceleration, sizeof(accel_mps2)); // Copies as g's
    memcpy(  &w_rps,  &gyro_event.gyro, sizeof(w_rps)); // Copies as deg/s
    memcpy(  &temp_C,  &temp_event.temperature, sizeof(temp_C)); // Copies as degC
    memcpy(  &B_uT,  &mag_event.magnetic, sizeof(B_uT)); // Copies as uT
    
    DEBUG("Temp:");
    DEBUG(temp_C);
    DEBUG("degC, ");


    DEBUG("\t\tAccel X: ");
    DEBUG(accel_mps2[0][0]);
    DEBUG(", Y: ");
    DEBUG(accel_mps2[1][0]);
    DEBUG(", Z: ");
    DEBUG(accel_mps2[2][0]);
    DEBUG(" g, ");

    DEBUG("\t\tGyro X: ");
    DEBUG(w_rps[0][0]);
    DEBUG(", Y: ");
    DEBUG(w_rps[1][0]);
    DEBUG(", Z: ");
    DEBUG(w_rps[2][0]);
    DEBUGLN(" deg/s, ");

    DEBUG("\t\tMag X: ");
    DEBUG(B_uT[0][0]);
    DEBUG(", Y: ");
    DEBUG(B_uT[1][0]);
    DEBUG(", Z: ");
    DEBUG(B_uT[2][0]);
    DEBUGLN(" uT");

    // Serial.print(mag_event.magnetic.x);
    // Serial.print(","); Serial.print(mag_event.magnetic.y);
    // Serial.print(","); Serial.print(mag_event.magnetic.z);
    return false;
}

}
}
