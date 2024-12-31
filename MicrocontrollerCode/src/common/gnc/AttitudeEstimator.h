#pragma once

#include "../math/vector.h"

namespace Cesium {

class AttitudeEstimator {
private:
    Vector3<float>* imu_a_accel_body; // m/s2
    Vector3<float>* imu_a_gyro_body; // rad/s

    Vector3<float>* imu_b_accel_body; // m/s2
    Vector3<float>* imu_b_gyro_body; // rad/s

    Matrix3<float> body_to_launch_initial;

public:
    AttitudeEstimator();

    bool config(Matrix3<float> body_to_launch_initial,
                Vector3<float>* imu_accel_body, Vector3<float>* imu_gyro_body);
                
    bool config(Matrix3<float> body_to_launch_initial,
                Vector3<float>* imu_a_accel_body, Vector3<float>* imu_a_gyro_body, 
                Vector3<float>* imu_b_accel_body, Vector3<float>* imu_b_gyro_body);

    bool run();

    Matrix3<float> body_to_launch;

};


}