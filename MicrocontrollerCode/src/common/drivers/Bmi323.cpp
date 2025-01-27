#include "Bmi323.h"

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

    return false;
}
bool Bmi323::read()
{
    return false;
}


bool Bmi323::readAccelerometer(float& x, float& y, float& z) {
    return true;
}

bool Bmi323::readGyroscope(float& x, float& y, float& z) {
    return true;
}

/******************************************************************************/
/*!           Static Function Definitions from BOSCH API                            */

int8_t Bmi323::set_accel_config(bmi3_dev *dev)
{
    /* Status of API are returned to this variable. */
    int8_t rslt;

    // /* Structure to define accelerometer configuration. */
    // struct bmi3_sens_config config;

    // /* Structure to map interrupt */
    // struct bmi3_map_int map_int = { 0 };

    // /* Configure the type of feature. */
    // config.type = BMI323_ACCEL;

    // /* Get default configurations for the type of feature selected. */
    // rslt = bmi323_get_sensor_config(&config, 1, dev);
    // bmi3_error_codes_print_result("bmi323_get_sensor_config", rslt);

    // if (rslt == BMI323_OK)
    // {
    //     map_int.acc_drdy_int = BMI3_INT1;

    //     /* Map data ready interrupt to interrupt pin. */
    //     rslt = bmi323_map_interrupt(map_int, dev);
    //     bmi3_error_codes_print_result("bmi323_map_interrupt", rslt);

    //     if (rslt == BMI323_OK)
    //     {
    //         /* NOTE: The user can change the following configuration parameters according to their requirement. */
    //         /* Output Data Rate. By default ODR is set as 100Hz for accel. */
    //         config.cfg.acc.odr = BMI3_ACC_ODR_100HZ;

    //         /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G). */
    //         config.cfg.acc.range = BMI3_ACC_RANGE_2G;

    //         /* The Accel bandwidth coefficient defines the 3 dB cutoff frequency in relation to the ODR. */
    //         config.cfg.acc.bwp = BMI3_ACC_BW_ODR_QUARTER;

    //         /* Set number of average samples for accel. */
    //         config.cfg.acc.avg_num = BMI3_ACC_AVG64;

    //         /* Enable the accel mode where averaging of samples
    //          * will be done based on above set bandwidth and ODR.
    //          * Note : By default accel is disabled. The accel will get enable by selecting the mode.
    //          */
    //         config.cfg.acc.acc_mode = BMI3_ACC_MODE_NORMAL;

    //         /* Set the accel configurations. */
    //         rslt = bmi323_set_sensor_config(&config, 1, dev);
    //         bmi3_error_codes_print_result("bmi323_set_sensor_config", rslt);
    //     }
    // }
    rslt = 0;

    return rslt;
}

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
float lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (dps / (half_scale)) * (val);
}


void bmi3_error_codes_print_result(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BMI3_OK:

            /*! Do nothing */
            break;

        case BMI3_E_NULL_PTR:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Null pointer error. It occurs when the user tries to assign value (not address) to a pointer," " which has been initialized to NULL.\r\n",
                rslt);
            break;

        case BMI3_E_COM_FAIL:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Communication failure error. It occurs due to read/write operation failure and also due " "to power failure during communication\r\n",
                rslt);
            break;

        case BMI3_E_DEV_NOT_FOUND:
            printf("%s\t", api_name);
            printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                   rslt);
            break;

        case BMI3_E_INVALID_SENSOR:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid sensor error. It occurs when there is a mismatch in the requested feature with the " "available one\r\n",
                rslt);
            break;

        case BMI3_E_INVALID_INT_PIN:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid interrupt pin error. It occurs when the user tries to configure interrupt pins " "apart from INT1 and INT2\r\n",
                rslt);
            break;

        case BMI3_E_ACC_INVALID_CFG:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid Accel configuration error. It occurs when there is an error in accel configuration" " register which could be one among range, BW or filter performance in reg address 0x20\r\n",
                rslt);
            break;

        case BMI3_E_GYRO_INVALID_CFG:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid Gyro configuration error. It occurs when there is a error in gyro configuration" "register which could be one among range, BW or filter performance in reg address 0x21\r\n",
                rslt);
            break;

        case BMI3_E_INVALID_INPUT:
            printf("%s\t", api_name);
            printf("Error [%d] : Invalid input error. It occurs when the sensor input validity fails\r\n", rslt);
            break;

        case BMI3_E_INVALID_STATUS:
            printf("%s\t", api_name);
            printf("Error [%d] : Invalid status error. It occurs when the feature/sensor validity fails\r\n", rslt);
            break;

        case BMI3_E_DATA_RDY_INT_FAILED:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Data ready interrupt error. It occurs when the sample count exceeds the FOC sample limit " "and data ready status is not updated\r\n",
                rslt);
            break;

        case BMI3_E_INVALID_FOC_POSITION:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid FOC position error. It occurs when average FOC data is obtained for the wrong" " axes\r\n",
                rslt);
            break;

        case BMI3_E_INVALID_ST_SELECTION:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Invalid self-test selection error. It occurs when there is an invalid precondition" "settings such as alternate accelerometer and gyroscope enable bits, accelerometer mode and output data rate\r\n",
                rslt);
            break;

        case BMI3_E_OUT_OF_RANGE:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Out of range error. It occurs when the range exceeds the maximum range for accel while performing FOC\r\n",
                rslt);
            break;

        case BMI3_E_FEATURE_ENGINE_STATUS:
            printf("%s\t", api_name);
            printf(
                "Error [%d] : Feature engine status error. It occurs when the feature engine enable mask is not set\r\n",
                rslt);
            break;

        default:
            printf("%s\t", api_name);
            printf("Error [%d] : Unknown error code\r\n", rslt);
            break;
    }
}

}
}