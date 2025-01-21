#include "SensorBase.h"
#include "../../math/vector.h"

namespace Cesium {
namespace Sensor {
class AccelerometerBase : public SensorBase {

private:
    float accel_mps[3];
    float accel_body_mps[3];
    Matrix3<float> sensor_to_body;

public:

    AccelerometerBase();
    virtual ~AccelerometerBase() = 0;


    
};


} // namespace Sensor
} // namespace Cesium