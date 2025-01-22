#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class AccelerometerBase : virtual public SensorBase {

protected:
    Vector3<float> accel_mps;
    Vector3<float> accel_body_mps;


public:

    AccelerometerBase();
    virtual ~AccelerometerBase();

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium