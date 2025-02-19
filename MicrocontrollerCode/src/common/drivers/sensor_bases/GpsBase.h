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

    uint32_t heading_scaled;
    uint32_t ground_speed_mmps;

public:

    GpsBase();
    virtual ~GpsBase();

    virtual bool read(); 
};


} // namespace Sensor
} // namespace Cesium