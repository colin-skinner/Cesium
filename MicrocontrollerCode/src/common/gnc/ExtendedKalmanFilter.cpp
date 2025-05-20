#include "ExtendedKalmanFilter.h"
#include "../math/vector.h"

namespace Cesium {

EkfState ExtendedKalmanFilter::state{};

bool ExtendedKalmanFilter::run(float delta_t)
{

    return false;
}

void ExtendedKalmanFilter::predict(float delta_t)
{
    // TODO: figure out how to get the imu data into here in the current data abstraction
    Vector3<float> a_body_corrected_1{};
    Vector3<float> a_body_corrected_2{};

    Vector3<float> w_body_corrected_1{};
    Vector3<float> w_body_corrected_2{};

    Vector3<float> a_body_fused = (a_body_corrected_1 + a_body_corrected_2) * 0.5f;
    Vector3<float> w_body_fused = (a_body_corrected_1 + a_body_corrected_2) * 0.5f;

    Quaternion<float> w_quat = {0, w_body_fused[0][0], w_body_fused[0][1], w_body_fused[0][2]};

    // q_dot = 1/2 * omega * q
    // body_to_launch_dot = quat_mult(w_quat, body_to_launch) * 0.5f;

    // // q_k+1 = q_k + q_dot
    // body_to_launch = body_to_launch + body_to_launch_dot * delta_t;

    // body_to_launch = unit(body_to_launch);

    // Vector3<float> a_launch = quat_apply(body_to_launch, w_body_fused);
    // a_launch[2][0] -= 9.81; // Gravity

}

}