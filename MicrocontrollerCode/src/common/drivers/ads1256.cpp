#include "ads1256.h"
#include "../globals.h"

namespace Cesium {

Ads1256::Ads1256(uint8_t cs_pin, uint8_t drdy_pin, SPIClass *spi_instance, size_t num_readings)
    : cs_pin(cs_pin)
    , drdy_pin(drdy_pin)
    , spi_instance_ptr(spi_instance)
    
{
    constrain(num_readings, 0, 8);
    channel_count = num_readings;
}

bool Ads1256::set_calibration(float *slopes, float *intercepts)
{

    float A_vals_GND[8] = {377.3854375, 377.5595104, 373.5603094, 3011.981888, 0, 0, 0, 0};
    float B_vals_GND[8] = {-254.1467792, -245.7410574, -253.5007837, -35.21335546, 0, 0, 0, 0};

    float A_vals_BAT[8] = {379.5779242, 378.3120511, 376.2891849, 3028.588266, 0, 0, 0, 0};
    float B_vals_BAT[8] = {-254.1467792, -245.7410574, -253.5007837, -35.21335546, 0, 0, 0, 0};
    

    if (slopes == nullptr) {
        slopes = A_vals_GND;
    }
    if (intercepts == nullptr) {
        intercepts = B_vals_GND;
    }


    for (size_t i = 0; i < channel_count; i++) {
        calibrate_slopes[i] = slopes[i];
        calibrate_intercepts[i] = intercepts[i];
    }
    
    return true;
}

bool Ads1256::setup()
{
    device.setup(drdy_pin, 0, 0, cs_pin, VREF);

    device.InitializeADC(spi_instance_ptr);

    device.setPGA(PGA_1);  //0b00000000 - DEC: 0
    device.setMUX(DIFF_0_1); //0b01100111 - DEC: 103
    device.setDRATE(DRATE_1000SPS); //0b00010011 - DEC: 19

    //Read back the above 3 values to check if the writing was succesful
    DEBUG(F("PGA (expected 0): "));
    DEBUGLN(device.readRegister(ADCON_REG));
    delay(100);
    //--
    DEBUG(F("MUX (expected 1): "));
    DEBUGLN(device.readRegister(MUX_REG));
    delay(100);
    //--
    DEBUG(F("DRATE (expected 161): "));
    DEBUGLN(device.readRegister(DRATE_REG));
    delay(100);

    return true;
}

bool Ads1256::read()
{
    
    for (int i = 0; i < channel_count; i++)
    {
        voltage_raw[i] = device.convertToVoltage(device.cycleSingle()); //print the converted single-ended results with 4 digits
        DEBUG(String(voltage_raw[i],4));
        DEBUG(",");
    }

    for (int i = channel_count; i < 8; i++) {
        device.cycleSingle();
    }
    DEBUGLN();//Printing a linebreak - this will put the next 8 conversions in a new line
        
    device.stopConversion();

    // Converting to volts
    for (int i = 0; i < channel_count; i++)
    {
        voltage_volt[i] = voltage_raw[i] * calibrate_slopes[i] + calibrate_intercepts[i];
        DEBUG(String(voltage_volt[i],4));
        DEBUG("V, ");
    }

    return true;


}

}