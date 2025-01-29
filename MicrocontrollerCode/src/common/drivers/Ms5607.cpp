#include "Ms5607.h"
#include "../globals.h"

namespace Cesium {
namespace Sensor {

Ms5607::Ms5607(uint8_t cs_pin, SPIClass *spi_instance) : P_offsets{}
{
    device.setCSpin(cs_pin);
    device.setSPIport(spi_instance);

    this->_spi_instance = spi_instance;
    this->interface = Interfaces::SPI;
    this->_cs_pin = cs_pin;
}

Ms5607::~Ms5607()
{
}

bool Ms5607::configure(const char* config_name) {

    return false;
}

bool Ms5607::calibrate(float current_alt_m)
{
    

    // Simple linear offset
    return false;
}

void Ms5607::set_P_offsets(float a, float b)
{
    P_offsets[0] = a;
    P_offsets[1] = b;
}

bool Ms5607::setup()
{
    if (device.begin())
    {
        DEBUG("MS5607 found: ");
        // DEBUGLN(ms5607.getDeviceID(), HEX);
    }
    else {
        DEBUG("MS5607 not found");
        return false;
    }

    if (device.reset(1))
    {
        DEBUGLN("Conversion set to MS5607");
    }
    
    return true;
}

bool Ms5607::read()
{
    if (device.read() != MS5611_READ_OK) {
        return false;
    }

    temp_C = device.getTemperature();
    pressure_kPa = device.getPressure() / 10;
    altitude_m = t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure_kPa / SEALEVELPRESSURE_KPA)));


    // DEBUG("Altimeter 2 (MS5607): ");
    DEBUG(temp_C);
    DEBUG(", ");
    // DEBUG(" *C, ");
    DEBUG(pressure_kPa);
    DEBUG(",");
    // DEBUG(" Pa, ");
    DEBUGLN(altitude_m);

    return true;
}


}
}

