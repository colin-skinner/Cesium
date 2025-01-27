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

    const Vector3<float>& get_w_rps() {return w_rps;}
    const Vector3<float>& get_w_body_rps() {return w_body_rps;}
};


} // namespace Sensor
} // namespace Cesium