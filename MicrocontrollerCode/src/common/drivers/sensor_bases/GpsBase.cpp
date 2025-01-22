#include "GpsBase.h"

namespace Cesium {
namespace Sensor {


GpsBase::GpsBase()
    : latitude_scaled{}
    , longitude_scaled{}
    , latitude_deg{}
    , longitude_deg{}
    , altitude_m{}
    , satellites_in_view{}
    , course_deg{}
    , horizontal_speed_mps{}
{}

bool GpsBase::read()
{
    // Sets last_read_time_ms; TODO: figure out if it is faster to just write it out
    return SensorBase::read();
}

} // namespace Sensor
} // namespace Cesium