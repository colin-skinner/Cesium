#include "MagnetometerBase.h"

namespace Cesium {
namespace Sensor {


MagnetometerBase::MagnetometerBase()
    : B_uT{}
    , B_body_uT{}
{}

MagnetometerBase::~MagnetometerBase() {}

bool MagnetometerBase::read()
{
    // Sets last_read_time_ms; TODO: figure out if it is faster to just write it out
    return SensorBase::read();
}

} // namespace Sensor
} // namespace Cesium