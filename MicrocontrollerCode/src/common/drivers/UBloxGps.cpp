#include "UBloxGps.h"
#include "../globals.h"
namespace Cesium {
namespace Sensor {
UBloxGps::UBloxGps(uint8_t cs_pin, SPIClass* spi_instance)
{
    this->cs_pin = cs_pin;
    this->spi_instance = spi_instance;
    this->interface = Interfaces::SPI;
    this->clock_freq = 4000000;
}

UBloxGps::UBloxGps(TwoWire* i2c_instance, uint8_t address)
{
    this->_i2c_instance = i2c_instance;
    this->interface = Interfaces::I2C;
    this->clock_freq = 100000;
    this->i2c_addr = address;
}

UBloxGps::UBloxGps(HardwareSerial* serial_instance)
{
    this->_serial_instance = serial_instance;
    this->interface = Interfaces::Serial;
}

bool UBloxGps::configure(const char *config_name)
{
    return false;
}

bool UBloxGps::setup()
{

    switch (interface) {
    case Interfaces::SPI:
        if (!device.begin(*spi_instance, cs_pin, clock_freq)) 
        {
            DEBUGLN("u-blox GNSS not detected on SPI bus. Please check wiring.");
            return false;
        }
        break;

    case Interfaces::I2C:
        if (!device.begin(*_i2c_instance, i2c_addr)) 
        {
            DEBUGLN("u-blox GNSS not detected on I2C bus. Please check wiring.");
            return false;
        }
        break;

    case Interfaces::Serial:
        if (!device.begin(*_serial_instance)) 
        {
            DEBUGLN("u-blox GNSS not detected on I2C bus. Please check wiring.");
            return false;
        }
        break;
    }

    device.factoryDefault(); delay(5000); // Uncomment this line to reset the module back to its factory defaults

    device.setPortOutput(COM_PORT_SPI, COM_TYPE_UBX); //Set the SPI port to output UBX only (turn off NMEA noise)
    device.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

    return true;
    
}

bool UBloxGps::read()
{

    latitude_scaled = device.getLatitude();
    DEBUG(F("Lat: "));
    DEBUG(latitude_scaled);

    longitude_scaled = device.getLongitude();
    DEBUG(F(" Long: "));
    DEBUG(longitude_scaled);
    DEBUG(F(" (degrees * 10^-7)"));

    altitude_m = device.getAltitude();
    DEBUG(F(" Alt: "));
    DEBUG(altitude_m);
    DEBUG(F(" (mm)"));

    satellites_in_view = device.getSIV();
    DEBUG(F(" SIV: "));
    DEBUG(satellites_in_view);

    heading_scaled = device.getHeading();
    DEBUG(F(" Heading: "));
    DEBUG(heading_scaled);

    ground_speed_mmps = device.getGroundSpeed(); // Scale this to altitude
    DEBUG(F(" Heading: "));
    DEBUG(ground_speed_mmps);

    DEBUGLN();
    return false;
}

}
}