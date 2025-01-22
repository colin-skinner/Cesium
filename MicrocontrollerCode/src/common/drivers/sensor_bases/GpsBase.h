#pragma once

#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class GpsBase : public SensorBase {

protected:
    uint32_t latitude_scaled;
    uint32_t longitude_scaled;

    float latitude_deg;
    float longitude_deg;

    float altitude_m;

    uint8_t satellites_in_view;

    float course_deg;
    float horizontal_speed_mps;

public:

    GpsBase();
    virtual ~GpsBase() = 0;

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium