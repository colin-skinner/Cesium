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

    const Vector3<float>& get_accel_mps() {return accel_mps;}
    const Vector3<float>& get_accel_body_mps() {return accel_body_mps;}
};


} // namespace Sensor
} // namespace Cesium