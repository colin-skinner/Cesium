#include "AttitudeEstimator.h"

namespace Cesium {
    
Quaternion<float> AttitudeEstimator::body_to_launch = {1,0,0,0};
Quaternion<float> AttitudeEstimator::body_to_launch_dot = {0,0,0,0};

std::vector<Sensor::AccelerometerBase*> AttitudeEstimator::accels{};
std::vector<Sensor::GyroscopeBase*> AttitudeEstimator::gyros{};
std::vector<Sensor::MagnetometerBase*> AttitudeEstimator::mags{};

Vector3<float> AttitudeEstimator::a_body_mps = {0.0 ,0.0 ,0.0};
Vector3<float> AttitudeEstimator::w_body_rps = {0.0 ,0.0 ,0.0};

Vector3<float> AttitudeEstimator::a_launch_mps = {0.0 ,0.0 ,0.0};
Vector3<float> AttitudeEstimator::w_launch_rps = {0.0 ,0.0 ,0.0};

void AttitudeEstimator::average_all_values()
{
    a_body_mps = {0.0, 0.0, 0.0};
    for (const auto& accelerometer : accels) {
        auto a_body = accelerometer->get_accel_body_mps2();

        // Averages acceleration components
        for (uint8_t i = 0; i<3; i++) {
            a_body_mps[i][0] += a_body[i][0] / accels.size();
        }
    }

    w_body_rps = {0.0, 0.0, 0.0};
    for (const auto& gyroscope : gyros) {
        auto w_body = gyroscope->get_w_body_rps();

        // Averages angular velocity components
        for (uint8_t i = 0; i<3; i++) {
            w_body_rps[i][0] += w_body[i][0] / gyros.size();
        }
    }
    
    // TODO: When magnetometers are implmented, do this too
}

void AttitudeEstimator::iterate_q()
{
    Quaternion<float> w_quat = {0, w_launch_rps[0][0], w_launch_rps[0][1], w_launch_rps[0][2]};

    // q_dot = 1/2 * omega * q
    body_to_launch_dot = quat_mult(w_quat, body_to_launch) * (float)0.5;

    // q_k+1 = q_k + q_dot
    // body_to_launch = body_to_launch + body_to_launch_dot;

}

void AttitudeEstimator::run(bool calc_w_launch)
{
    // Averages sensor readings in the body frame
    average_all_values();

    // Uses quaternion to apply to the vectors
    a_launch_mps = quat_apply(body_to_launch, a_body_mps);

    if (calc_w_launch) {
        w_launch_rps = quat_apply(body_to_launch, w_body_rps);
    }

    // Uses vectors to iterate q and q_dot
    iterate_q();

    // TODO: Runs magnetic field model calculations to get body to launch from magnetometers
}




}