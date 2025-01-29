#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class AccelerometerBase : virtual public SensorBase {

protected:
    Vector3<float> accel_mps2;
    Vector3<float> accel_body_mps2;


public:

    AccelerometerBase();
    virtual ~AccelerometerBase();

    virtual bool read(); 

    const Vector3<float>& get_accel_mps2() {return accel_mps2;}
    const Vector3<float>& get_accel_body_mps2() {return accel_body_mps2;}
};


} // namespace Sensor
} // namespace Cesium