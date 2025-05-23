#include "GyroscopeBase.h"

namespace Cesium {
namespace Sensor {


GyroscopeBase::GyroscopeBase()
    : w_rps{}
    , w_body_rps{}
{}

GyroscopeBase::~GyroscopeBase() {}

bool GyroscopeBase::read()
{
    // Sets last_read_time_ms; TODO: figure out if it is faster to just write it out
    return SensorBase::read();
}

} // namespace Sensor
} // namespace Cesium