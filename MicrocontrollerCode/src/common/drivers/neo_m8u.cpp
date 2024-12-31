#include "neo_m8u.h"
#include "../globals.h"
namespace Cesium {
NeoM8U::NeoM8U(uint8_t cs_pin, SPIClass* spi_instance)
{
    this->cs_pin = cs_pin;
    this->spi_instance = spi_instance;
    this->spi_freq = 4000000;
}

bool NeoM8U::setup()
{
    if (!device.begin(*spi_instance, cs_pin, spi_freq)) 
    {
        DEBUGLN("u-blox GNSS not detected on SPI bus. Please check wiring.");
        return false;
    }

    device.factoryDefault(); delay(5000); // Uncomment this line to reset the module back to its factory defaults

    device.setPortOutput(COM_PORT_SPI, COM_TYPE_UBX); //Set the SPI port to output UBX only (turn off NMEA noise)
    device.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

    return true;
    
}

bool NeoM8U::read()
{

    latitude = device.getLatitude();
    DEBUG(F("Lat: "));
    DEBUG(latitude);

    longitude = device.getLongitude();
    DEBUG(F(" Long: "));
    DEBUG(longitude);
    DEBUG(F(" (degrees * 10^-7)"));

    altitude_m = device.getAltitude();
    DEBUG(F(" Alt: "));
    DEBUG(altitude_m);
    DEBUG(F(" (mm)"));

    byte satellites_in_view = device.getSIV();
    DEBUG(F(" SIV: "));
    DEBUG(satellites_in_view);

    DEBUGLN();
    return false;
}

}