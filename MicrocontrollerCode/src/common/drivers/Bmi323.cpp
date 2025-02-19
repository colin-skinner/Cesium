#include "Bmi323.h"
#include "../globals.h"
extern "C" {
#include "bosch_bmi3.h"
}

namespace Cesium {
namespace Sensor {


Bmi323::Bmi323()
{
    this->_spi_instance = &SPI;
    this->_i2c_instance = nullptr;
    this->_cs_pin = 0xFF;
    zero_structs();
}

// (Initilize sensor for I2C Communication)
Bmi323::Bmi323(TwoWire* i2c_instance)
{
    this->_spi_instance = nullptr;
    this->_i2c_instance = i2c_instance;
    this->_cs_pin = 0xFF;
    zero_structs();
}

// (Initilize sensor for SPI Communication)
Bmi323::Bmi323(uint8_t cs_pin, SPIClass *spi_instance)
{
    this->_spi_instance = spi_instance;
    this->_i2c_instance = nullptr;
    this->_cs_pin = cs_pin;
    zero_structs();
}

// Zero out structs
bool Bmi323::zero_structs()
{
    memset(&dev, 0, sizeof(dev));
    memset(&sensor_data, 0, sizeof(sensor_data));
    memset(&config, 0, sizeof(config));

    return true;
}


bool Bmi323::configure(const char *config_name)
{
    // TODO: Add configuration JSON
    return false;
}
bool Bmi323::setup()
{
    pinMode(_cs_pin, OUTPUT);

    uint16_t buff;
    readReg(0, &buff, 0);

    /*
    Configure the accelerometer with or'ed parameters
    - Normal mode: 0x4000
    - No averaging: 0x0000
    - Filtering to ODR/2: 0x0000
    - Range 8g: 0x0020
    - ODR 50Hz: 0x0007
    */
    uint16_t buff_acc[1] = {0x4027};
    writeReg(BMI3_REG_ACC_CONF, buff_acc, 1);


    /*
    Configure the gyroscope with or'ed parameters
    - Normal mode: 0x4000
    - No averaging: 0x0000
    - Filtering to ODR/2: 0x0000
    - Range 2kdps: 0x0040
    - ODR 800Hz: 0x000B
    */
    uint16_t buff_gyr[1] = {0x404B};
    writeReg(BMI3_REG_GYR_CONF, buff_gyr, 1);



    return true;
}
bool Bmi323::read()
{
    readAccelerometer((float*) &accel_mps2);
    readGyroscope((float*) &w_rps);

    DEBUG(accel_mps2[0][0]);
    DEBUG("\t");
    DEBUG(accel_mps2[0][1]);
    DEBUG("\t");
    DEBUG(accel_mps2[0][2]);
    DEBUG("\t");

    DEBUG(w_rps[0][0]);
    DEBUG("\t");
    DEBUG(w_rps[0][1]);
    DEBUG("\t");
    DEBUGLN(w_rps[0][2]);

    return true;
}

bool Bmi323::read_chip_id()
{
    uint16_t buffer[1] = {0};

    readReg(BMI3_REG_CHIP_ID, buffer, 1);

    Serial.println(buffer[0], HEX);

    return true;
}


bool Bmi323::readAccelerometer(float& x, float& y, float& z) {

    uint16_t buffer[3] = {0};

    readReg(BMI3_REG_ACC_DATA_X, buffer, 3);

    x = lsb_to_mps2(buffer[0], BMI3_ACC_8G, 16);
    y = lsb_to_mps2(buffer[1], BMI3_ACC_8G, 16);
    z = lsb_to_mps2(buffer[2], BMI3_ACC_8G, 16);

    return true;
}

bool Bmi323::readGyroscope(float& x, float& y, float& z) {

    uint16_t buffer[3] = {0};

    readReg(BMI3_REG_GYR_DATA_X, buffer, 3);

    x = lsb_to_dps(buffer[0], 2000, 16);
    y = lsb_to_dps(buffer[1], 2000, 16);
    z = lsb_to_dps(buffer[2], 2000, 16);

    return true;
}

bool Bmi323::readAccelerometer(float *buff)
{
    uint16_t buffer[3] = {0};

    readReg(BMI3_REG_ACC_DATA_X, buffer, 3);

    buff[0] = lsb_to_mps2(buffer[0], BMI3_ACC_8G, 16);
    buff[1] = lsb_to_mps2(buffer[1], BMI3_ACC_8G, 16);
    buff[2] = lsb_to_mps2(buffer[2], BMI3_ACC_8G, 16);


    return true;
}

bool Bmi323::readGyroscope(float *buff)
{
    uint16_t buffer[3] = {0};

    readReg(BMI3_REG_GYR_DATA_X, buffer, 3);

    buff[0] = lsb_to_dps(buffer[0], 2000, 16);
    buff[1] = lsb_to_dps(buffer[1], 2000, 16);
    buff[2] = lsb_to_dps(buffer[2], 2000, 16);

    return true;
}

bool Bmi323::readReg(uint8_t reg, uint16_t *buffer, size_t len)
{
    digitalWrite(_cs_pin, LOW);
    _spi_instance->beginTransaction(settings);

    _spi_instance->write(BMI3_SPI_RD_MASK | reg);
    _spi_instance->write(0);
    
    for (size_t i = 0; i < len; i++) {
        buffer[i] = _spi_instance->transfer(0);
        buffer[i] |= (uint16_t)_spi_instance->transfer(0) << 8;
    }

    _spi_instance->endTransaction();

    digitalWrite(_cs_pin, HIGH);

    return true;
}

bool Bmi323::writeReg(uint8_t reg, const uint16_t *buffer, size_t len)
{
    digitalWrite(_cs_pin, LOW);
    _spi_instance->beginTransaction(settings);

    _spi_instance->write(BMI3_SPI_WR_MASK & reg);
    
    for (size_t i = 0; i < len; i++) {
        _spi_instance->transfer(buffer[i] & 0xFF);
        _spi_instance->transfer( (buffer[i] >> 8) & 0xFF);
    }

    _spi_instance->endTransaction();

    digitalWrite(_cs_pin, HIGH);

    return true;
}


/******************************************************************************/
/*!           Static Function Definitions from BOSCH API                            */

float Bmi323::lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (GRAVITY_EARTH * val * g_range) / half_scale;
}

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 */
float Bmi323::lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (dps / (half_scale)) * (val);
}

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 */
float Bmi323::lsb_to_rps(int16_t val, float dps, uint8_t bit_width)
{
    return lsb_to_dps(val, dps, bit_width) * DEG2RAD;
}


}
}