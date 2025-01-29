#include "BarometerBase.h"

namespace Cesium {
namespace Sensor {


BarometerBase::BarometerBase()
    : altitude_m{}
    , pressure_kPa{}
{}

BarometerBase::~BarometerBase()
{
}

bool BarometerBase::read()
{
    // Sets last_read_time_ms; TODO: figure out if it is faster to just write it out
    return SensorBase::read();
}

} // namespace Sensor
} // namespace Cesium