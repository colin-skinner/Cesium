#include "AccelerometerBase.h"

namespace Cesium {
namespace Sensor {


AccelerometerBase::AccelerometerBase()
    : accel_mps{0}
    , accel_body_mps{0}
    , sensor_to_body{} {}


} // namespace Sensor
} // namespace Cesium