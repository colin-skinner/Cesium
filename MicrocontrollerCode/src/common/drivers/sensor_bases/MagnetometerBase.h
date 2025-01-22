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
    virtual ~MagnetometerBase() = 0;

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium