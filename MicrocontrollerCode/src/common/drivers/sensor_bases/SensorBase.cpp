#include "SensorBase.h"

namespace Cesium {
namespace Sensor {

SensorBase::SensorBase()
    : last_read_time_ms{0}
    , read_interval_ms{1000} // 1 second
    , _i2c_instance{nullptr}
    , _spi_instance{nullptr}
    , _serial_instance{nullptr}
    , interface{Interfaces::NOT_SET}
    , body_to_sensor{}
    , temp_C{NAN}
{}

SensorBase::~SensorBase() {}

void SensorBase::attach_I2C(TwoWire *I2C)
{
    _i2c_instance = I2C;
    interface = Interfaces::I2C;
}

void SensorBase::attach_SPI(SPIClass *spi)
{
    _spi_instance = spi;
    interface = Interfaces::SPI;
}

void SensorBase::attach_serial(HardwareSerial *serial)
{
    _serial_instance = serial;
    interface = Interfaces::Serial;
}

// Gets current milliseconds
bool SensorBase::read()
{
    last_read_time_ms = millis();

    return true;
}

void SensorBase::attach_int_pin(uint8_t pin, void (*func)(), uint8_t mode)
{
    attachInterrupt(digitalPinToInterrupt(pin), func, mode);
}

} // namespace Sensor
} // namespace Cesium