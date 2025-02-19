#pragma once

#include "../math/vector.h"
#include "../math/quaternion.h"

#include "../drivers/sensor_bases/AccelerometerBase.h"
#include "../drivers/sensor_bases/GyroscopeBase.h"
#include "../drivers/sensor_bases/MagnetometerBase.h"

/**
 * - Averages accelerometer, gyroscope, and magnetometer data
 * - Holds current rotation quaternion
 * - Turns body into global (launch frame)
 */

namespace Cesium {

class AttitudeEstimator {
private:
    // Vector3<float>* imu_a_accel_body; // m/s2
    // Vector3<float>* imu_a_gyro_body; // rad/s
    Quaternion<float> body_to_launch;

    static std::vector<Sensor::AccelerometerBase*> accels;
    static std::vector<Sensor::GyroscopeBase*> gyros;
    static std::vector<Sensor::MagnetometerBase*> mags;

    // Matrix3<float> body_to_launch_initial;

public:
    // AttitudeEstimator();

    static inline void add_accel(Sensor::AccelerometerBase* accel) {accels.push_back(accel);}
    static inline void add_gyro(Sensor::GyroscopeBase* gyro) {gyros.push_back(gyro);}
    static inline void add_mag(Sensor::MagnetometerBase* mag) {mags.push_back(mag);}
    
    // bool config(Matrix3<float> body_to_launch_initial,
    //             Vector3<float>* imu_accel_body, Vector3<float>* imu_gyro_body);
                
    // bool config(Matrix3<float> body_to_launch_initial,
    //             Vector3<float>* imu_a_accel_body, Vector3<float>* imu_a_gyro_body, 
    //             Vector3<float>* imu_b_accel_body, Vector3<float>* imu_b_gyro_body);

    // bool run();

    // Matrix3<float> body_to_launch;

};


}