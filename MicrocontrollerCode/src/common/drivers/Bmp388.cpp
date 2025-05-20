#include "Bmp388.h"
#include "../globals.h"

namespace Cesium {
namespace Sensor {

Bmp388::Bmp388()
{

    temp_C = 0;
}

Bmp388::Bmp388(uint8_t cs_pin, SPIClass * spi_instance)
{
    _spi_instance = spi_instance;
    interface = Interfaces::SPI;
    this->cs_pin = cs_pin;
}

void Bmp388::set_sea_level_pressure(float sea_level_pressure_hPa)
{
    _sea_level_pressure_hPa = sea_level_pressure_hPa;
}

bool Bmp388::configure(const char * config_name)
{
    return false;
}

bool Bmp388::setup()
{
    DEBUGLN("BRUH");
    size_t retries = 0;
    while(!device.begin_SPI(cs_pin, _spi_instance)) {
        retries++;

        if (retries >= 300) {
            DEBUGLN("Failed to find sensor");
            return false;
        }

        delay(10);
    }

    device.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    device.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    device.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    device.setOutputDataRate(BMP3_ODR_50_HZ);
    return true;
}

bool Bmp388::read()
{
    DEBUGLN("BRUH");
    if (! device.performReading()) {
        DEBUG("Failed to perform reading on BMI388");
        return false;
    }
    else {
        temp_C = device.temperature;
        pressure_kPa = device.pressure / 1000.0;
        altitude_m = device.readAltitude(_sea_level_pressure_hPa);
        
        // DEBUG("Altimeter 2 (MS5607): ");
        DEBUG(temp_C);
        DEBUG(", ");
        // DEBUG(" *C, ");
        DEBUG(pressure_kPa);
        DEBUG(",");
        // DEBUG(" Pa, ");
        DEBUGLN(altitude_m);
    }
    

    return true;
}









} // namespace Sensor
} // namespace Cesium