#include "icm20948.h"
#include "../globals.h"

namespace Cesium {

Icm20948::Icm20948()
{
    this->spi_instance_ptr = &SPI;
    this->cs_pin = 0;
}

Icm20948::Icm20948(uint8_t cs_pin, SPIClass* spi_instance)
{
    this->spi_instance_ptr = spi_instance;
    this->cs_pin = cs_pin;
}

bool Icm20948::setup()
{
    bool setup = false;
    for (int i=0; i<3000; i++) {
        if(device.begin_SPI(cs_pin, spi_instance_ptr)) {
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
    
    return false;
}

bool Icm20948::read()
{
    /* Get a new normalized sensor event */
    sensors_event_t accel_event;
    sensors_event_t gyro_event;
    sensors_event_t temp_event;
    sensors_event_t mag_event;
    device.getEvent(&accel_event, &gyro_event, &temp_event, &mag_event);

    this->temp = temp_event.temperature;
    this->accel = accel_event.acceleration;
    this->gyro = gyro_event.gyro;
    this->B_field = mag_event.magnetic;
    
    DEBUG("Temp:");
    DEBUG(temp);
    DEBUG("*C, ");


    DEBUG("\t\tAccel X: ");
    DEBUG(accel.x);
    DEBUG(", Y: ");
    DEBUG(accel.y);
    DEBUG(", Z: ");
    DEBUG(accel.z);
    DEBUG(" m/s^2, ");

    DEBUG("\t\tGyro X: ");
    DEBUG(gyro.x);
    DEBUG(", Y: ");
    DEBUG(gyro.y);
    DEBUG(", Z: ");
    DEBUG(gyro.z);
    DEBUGLN(" radians/s");
    return false;
}

}
