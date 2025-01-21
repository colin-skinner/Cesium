#include "SensorBase.h"

namespace Cesium {
namespace Sensor {

SensorBase::SensorBase() 
    : last_read_time_ms{0}
    , read_interval_ms{1000} // 1 second
    , _i2c_instance{nullptr}
    , _spi_instance{nullptr}
    , _serial_instance{nullptr} {}
                                                            
bool SensorBase::read()
{
    last_read_time_ms = millis();

    return true;
}

void SensorBase::attach_int_pin(uint8_t pin, void (*func)(), bool mode)
{
    attachInterrupt(digitalPinToInterrupt(pin), func, mode);
}

} // namespace Sensor
} // namespace Cesium