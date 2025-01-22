#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class GyroscopeBase : virtual public SensorBase {

protected:
    Vector3<float> w_rps;
    Vector3<float> w_body_rps;

public:

    GyroscopeBase();
    virtual ~GyroscopeBase() = 0;

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium