#include "ms5611.h"
#include "../globals.h"

namespace Cesium {
    
Ms5611::Ms5611(uint8_t cs_pin, SPIClass *spi_instance)
{
    this->device = MS5611_SPI(cs_pin, spi_instance);
}

bool Ms5611::setup()
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

    // if (device.reset(1))
    // {
    //     DEBUG("MS5611 conversion set to MS5607");
    // }
    
    return true;
}
bool Ms5611::read()
{
    if (device.read() != MS5611_READ_OK) {
        return false;
    }

    temp = device.getTemperature();
    pressure = device.getPressure();
    altitude = t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / SEALEVELPRESSURE_HPA)));


    // DEBUG("Altimeter 2 (MS5607): ");
    DEBUG(temp);
    DEBUG(", ");
    // DEBUG(" *C, ");
    DEBUG(pressure);
    DEBUG(",");
    // DEBUG(" Pa, ");
    DEBUGLN(altitude);

    return true;
}
}