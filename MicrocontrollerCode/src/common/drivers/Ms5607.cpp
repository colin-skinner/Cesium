#include "Ms5607.h"
#include "../globals.h"

namespace Cesium {
namespace Sensor {

Ms5607::Ms5607(uint8_t cs_pin, SPIClass *spi_instance) : offsets{}
{
    this->device = MS5611_SPI(cs_pin, spi_instance);
}

Ms5607::~Ms5607()
{
}

bool Ms5607::configure(const char* config_name) {

    return false;
}

bool Ms5607::calibrate(float current_alt_m)
{
    // Use GNC altitude if 0
    if (current_alt_m == 0) {

    }
    // Otherwise use GPS altitude
    else if (current_alt_m == -1) {
        // Make sure GPS altitude updated within the past second
    }

    // Simple linear offset
    return false;
}

bool Ms5607::calibrate(float offsets[2])
{
    // NO CHECKING to see if this is 2 long
    memcpy(this->offsets, offsets, 2 * sizeof(float));

    return false;
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

