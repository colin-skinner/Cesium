#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class BarometerBase : public SensorBase {

protected:
    float altitude_m;
    float pressure_kPa;

public:

    BarometerBase();
    virtual ~BarometerBase() = 0;

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium