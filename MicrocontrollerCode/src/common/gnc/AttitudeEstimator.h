#pragma once

/** 
 * Attitude Estimator Navigation Algorithm
 * 
 * 
 * 
 * 
 */
#include "../math/vector.h"
#include "../math/quaternion.h"

#include "../drivers/sensor_bases/AccelerometerBase.h"
#include "../drivers/sensor_bases/GyroscopeBase.h"
#include "../drivers/sensor_bases/MagnetometerBase.h"

/**
 * - Averages accelerometer, gyroscope, and magnetometer data
 * - Holds current rotation quaternion
 * - Turns body into global (launch frame)
 *     - Subtracts gravity from acceleration vector
 */

namespace Cesium {

class AttitudeEstimator {
private:
    // Vector3<float>* imu_a_accel_body; // m/s2
    // Vector3<float>* imu_a_gyro_body; // rad/s
    static Quaternion<float> body_to_launch;
    static Quaternion<float> body_to_launch_dot;

    static std::vector<Sensor::AccelerometerBase*> accels;
    static std::vector<Sensor::GyroscopeBase*> gyros;
    static std::vector<Sensor::MagnetometerBase*> mags;

    static Vector3<float> a_body_mps;
    static Vector3<float> w_body_rps;
    // Magnetometers also

    static Vector3<float> a_launch_mps;
    static Vector3<float> w_launch_rps;
    // Magnetometers also

    static void average_all_values();
    static void iterate_q();


public:

    static inline void add_accel(Sensor::AccelerometerBase* accel) {accels.push_back(accel);}
    static inline void add_gyro(Sensor::GyroscopeBase* gyro) {gyros.push_back(gyro);}
    static inline void add_mag(Sensor::MagnetometerBase* mag) {mags.push_back(mag);}

    const static Vector3<float>& get_a_launch_mps() { return a_launch_mps; }
    const static Vector3<float>& get_w_body_rps() { return w_body_rps; }
    const static Vector3<float>& get_w_launch_rps() { return w_launch_rps; }
    const static Quaternion<float>& get_body_to_launch() { return body_to_launch; }
    const static Quaternion<float>& get_body_to_launch_dot() { return body_to_launch_dot; }

    static inline void set_initial_launch(Quaternion<float> body_to_launch_initial) { body_to_launch = body_to_launch_initial; }

    static void run(bool calc_w_launch = false);

};


}