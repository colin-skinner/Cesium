#pragma once

#include "../math/vector.h"
namespace Cesium {

class ImuBase {
private:
    float* accel_ptr;
    float* gyro_ptr;
public:
    ImuBase();

    bool config(Matrix3<float> imu_to_body, float* accel_array, float* gyro_array);

    bool run();

    

    Vector3<float> accel;
    Vector3<float> gyro;
    Matrix3<float> imu_to_body;

    Vector3<float> accel_body;
    Vector3<float> gyro_body;

};

// class AltimeterBase {
// public:
//     AltimeterBase();

//     bool config(float* z_ptr);

//     bool run();

//     float* z_ptr;

// };


}