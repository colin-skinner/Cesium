#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class MagnetometerBase : virtual public SensorBase {

protected:
    Vector3<float> B_uT;
    Vector3<float> B_body_uT;

public:

    MagnetometerBase();
    virtual ~MagnetometerBase();

    virtual bool read(); 

    const Vector3<float>& get_B_uT() {return B_uT;};
    const Vector3<float>& get_B_body_uT() {return B_body_uT;};
};


} // namespace Sensor
} // namespace Cesium