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

#include "AttitudeEstimator.h"

/**
 * - State vector (16 parts)
 *     - position (3) [0:2]
 *     - velocity (3) [3:5]
 *     - quaternion (4) [6:9]
 *     - accel bias1 (3) [10:12]
 *     - gyro bias1 (3) [13:15]
 *     - accel bias2 (3) [16:18]
 *     - gyro bias2 (3) [19:22]
 * - Iterates on launch values for accel and gyro
 * - 
 */

namespace Cesium {

struct EkfState {
    union {
        struct {
            Vector3<float> position;    // m
            Vector3<float> velocity;    // m/s
            Quaternion<float> attitude; // World to Body rotation
            Vector3<float> accel_bias_imu[2]; // Bias for IMU1
            Vector3<float> gyro_bias_imu[2]; // Bias for IMUs
        };
        float data[22];
    };
};



class ExtendedKalmanFilter {
private:

static EkfState state;

static Matrix<float, 22, 22> P;
static Matrix<float, 22, 22> Q;
static Matrix<float, 22, 22> R; // Measurement noise



public:

static bool run(float delta_t);
static void predict(float delta_t);


};


}