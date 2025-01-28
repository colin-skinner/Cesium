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
    for (int i=0; i<300; i++) {
        if(device.begin_SPI(cs_pin, _spi_instance))
            break;    
        DEBUGLN("Failed to find BMP 1");
    }
    return false;
}

bool Bmp388::read()
{
    if (! device.performReading()) {
        DEBUG("Failed to perform reading on BMI388");
        return false;
    }
    else {
        temp_C = device.temperature;
        pressure_kPa = device.pressure / 1000.0;
        altitude_m = device.readAltitude(_sea_level_pressure_hPa);
    }
    return true;
}









} // namespace Sensor
} // namespace Cesium