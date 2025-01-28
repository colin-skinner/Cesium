#include "arduino_bmi323.h"

#ifndef RETURN_FALSE_IF_FALSE 
    #define RETURN_FALSE_IF_FALSE(condition) if (!(condition)) {return false;}
#endif

#ifndef RETURN_0_IF_FALSE 
    #define RETURN_0_IF_FALSE(condition) if (!(condition)) {return 0;}
#endif
// BMI323::BMI323()
// {
// }

BMI323::BMI323(uint8_t cs_pin, SPIClass *spi_instance)
{
    this->_cs_pin = cs_pin;
    this->_spi_instance = spi_instance;
    this->chip_id = 0;
    digitalWrite(_cs_pin, HIGH);  
}

uint8_t BMI323::read_chip_id()
{
    uint16_t id_rslt;
    
    // Get chip id
    RETURN_0_IF_FALSE(read_reg(BMI3_REG_CHIP_ID, &id_rslt, 1) == 0);

    // 16 bit read to 8 bit
    uint8_t id = id_rslt & 0xFF;

    return true;
}

int8_t BMI323::initialize()
{
    // SPI dummy read
    uint16_t dummy;
    read_reg(0, &dummy, 1);


    // uint16_t reset_command = BMI3_CMD_SOFT_RESET;
    // // Soft reset by setting command register to BMI3_CMD_SOFT_RESET
    // RETURN_FALSE_IF_FALSE(write_reg(BMI3_REG_CMD, &reset_command, 1));
    // delayMicroseconds(2000);

    // Assert chip id matches
    uint8_t id = read_chip_id();
    if (id != BMI323_CHIP_ID) {
        Serial.println("BMI ID: " + String(id) + ", expected " + BMI323_CHIP_ID);
        return BMI3_E_COM_FAIL;
    }

     // Error Status register
    uint16_t err_status;
    if (read_reg(BMI3_REG_ERR_REG, &err_status, 1) == 0) {
        Serial.println("BMI error status not read");
        return BMI3_E_COM_FAIL;
    }

    uint16_t expected_status = 0b0;
    if (err_status != expected_status) {
        Serial.println("Power error. Status: " + String(err_status) + ", expected " + expected_status);
        return BMI3_E_INVALID_STATUS;
    }

    // Status register
    uint16_t status;
    if (read_reg(BMI3_REG_ERR_REG, &status, 1) == 0) {
        Serial.println("BMI status not read");
        return BMI3_E_COM_FAIL;
    }

    expected_status = 0b1;
    if (status != expected_status) {
        Serial.println("Initialization error. Status: " + String(status) + ", expected " + expected_status);
        return BMI3_E_INVALID_STATUS;
    }

    // Configure acceleration
    uint16_t configs[2];
    configs[0] =  (BMI3_ACC_MODE_NORMAL << BMI3_ACC_MODE_POS);
    configs[0] |= (BMI3_ACC_AVG1 << BMI3_ACC_AVG_NUM_POS);
    configs[0] |= (BMI3_ACC_BW_ODR_HALF << BMI3_ACC_BW_POS);
    configs[0] |= (BMI3_ACC_RANGE_8G << BMI3_ACC_RANGE_POS);
    configs[0] |= (BMI3_ACC_ODR_50HZ);

    // if (write_reg(BMI3_REG_ACC_CONF, &configs[0], 1) == 0) {
    //     Serial.println("BMI acc config not written");
    //     return BMI3_E_COM_FAIL;
    // }

    // Configure gyroscope
    configs[1] =  (BMI3_GYR_MODE_NORMAL << BMI3_ACC_MODE_POS);
    configs[1] |= (BMI3_GYR_AVG1 << BMI3_GYR_AVG_NUM_POS);
    configs[1] |= (BMI3_GYR_BW_ODR_HALF << BMI3_GYR_BW_POS);
    configs[1] |= (BMI3_GYR_RANGE_2000DPS << BMI3_ACC_RANGE_POS);
    configs[1] |= (BMI3_GYR_ODR_400HZ);

    if (write_reg(BMI3_REG_ACC_CONF, configs, 2) == 0) {
        Serial.println("BMI configs not written");
        return BMI3_E_COM_FAIL;
    }



    return true;
}

bool BMI323::read_reg(uint8_t reg, uint16_t *rx_buffer, size_t size)
{
    // Start transferring data
    _spi_instance->beginTransaction(_spi_settings);
    digitalWrite(_cs_pin, LOW);  

    // Register
    _spi_instance->write(reg & BMI3_SPI_RD_MASK);

    // Dummy byte (pg. 200)
    _spi_instance->transfer(0);

    // To hold LSB while MSB is received
    uint16_t temp;

    for (int i = 0; i < size; i++) {

        rx_buffer[i] = _spi_instance->transfer16(0);
        
    }

    digitalWrite(_cs_pin, HIGH);  
    _spi_instance->endTransaction();

    return false;
}

bool BMI323::write_reg(uint8_t reg, uint16_t *tx_buffer, size_t size)
{
    // Start transferring data
    _spi_instance->beginTransaction(_spi_settings);
    digitalWrite(_cs_pin, LOW);  

    // Register
    _spi_instance->write(reg & BMI3_SPI_WR_MASK);

    // To hold LSB while MSB is received
    uint16_t temp;

    for (int i = 0; i < size; i++) {

        _spi_instance->write16(tx_buffer[i]);
        
    }

    digitalWrite(_cs_pin, HIGH);  
    _spi_instance->endTransaction();

    return false;
}

float BMI323::lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (GRAVITY_EARTH * val * g_range) / half_scale;
}

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 */
float BMI323::lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (dps / (half_scale)) * (val);
}
