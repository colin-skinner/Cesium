#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class GyroscopeBase : virtual public SensorBase {

protected:
    Vector3<float> w_dps;
    Vector3<float> w_body_dps;

public:

    GyroscopeBase();
    virtual ~GyroscopeBase();

    virtual bool read(); 

    const Vector3<float>& get_w_dps() {return w_dps;}
    const Vector3<float>& get_w_body_dps() {return w_body_dps;}
};


} // namespace Sensor
} // namespace Cesium