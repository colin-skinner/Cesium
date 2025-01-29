#include "AccelerometerBase.h"

namespace Cesium {
namespace Sensor {


AccelerometerBase::AccelerometerBase()
    : accel_mps2{}
    , accel_body_mps2{}
{}

AccelerometerBase::~AccelerometerBase()
{
}

bool AccelerometerBase::read()
{
    // Sets last_read_time_ms; TODO: figure out if it is faster to just write it out
    return SensorBase::read();
}

} // namespace Sensor
} // namespace Cesium