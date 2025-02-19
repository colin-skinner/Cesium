#pragma once

#include "vector.h"
#include "../globals.h" // RAD2DEG and DEG2RAD
#include <math.h>
template <typename T> // float or double
using Quaternion = Vector<T, 4>;

//////////////////////  Vec as Quat //////////////////////
template <typename T>
Quaternion<T> vec_as_quat(const Vector3<T>& vec) {
    Quaternion<T> result{};
    result[1][0] = vec[0][0];
    result[2][0] = vec[1][0];
    result[3][0] = vec[2][0];
    return result;
}

// First element needs to be 0
template <typename T>
Vector3<T> quat_as_vec(const Quaternion<T>& quat) {

    // First element needs to be 0
    if (abs(quat[0][0]) > 0.001) {
        return Vector3<T>();
    }

    Vector3<T> result{};
    result[0][0] = quat[1][0];
    result[1][0] = quat[2][0];
    result[2][0] = quat[3][0];
    return result;
}

//////////////////////  Axis Rotation //////////////////////

// Quat from axis rot in float or double
template <typename T>
Quaternion<T> quat_from_axis_rot(float angle_deg, const Vector3<float>& axis) {
    if (abs(angle_deg) < 0.001) {
        // Zero angle, so return {1,0,0,0}
        return {1,0,0,0};
    }
    float angle_rad = angle_deg * DEG2RAD;
    Vector3<float> normalized_axis = unit(axis);
    Quaternion<T> result = {{
        {std::cos(angle_rad/2)},
        {std::sin(angle_rad/2) * normalized_axis[0][0]},
        {std::sin(angle_rad/2) * normalized_axis[1][0]},
        {std::sin(angle_rad/2) * normalized_axis[2][0]}
    }};

    return result;
}
// Axis rot from quat
template <typename T>
bool axis_rot_from_quat(const Quaternion<T>& quat, float& angle_deg, Vector3<float>& axis) {
    //TODO: bounds checking
    Quaternion<T> normalized_quat = unit(quat);

    float angle_rad = 2* std::acos(normalized_quat[0][0]);

    // 0 degrees
    if (angle_rad < 0.001) {
        memset(&axis, 0, size(axis));
        angle_deg = 0;
        return true;
    }

    // Else set everything else
    axis[0][0] = normalized_quat[1][0] / std::sin(angle_rad/2);
    axis[1][0] = normalized_quat[2][0] / std::sin(angle_rad/2);
    axis[2][0] = normalized_quat[3][0] / std::sin(angle_rad/2);

    angle_deg = angle_rad * RAD2DEG;

    return true;
}

//////////////////////  Rotation Matrix //////////////////////

// R from quat
template <typename T>
Matrix3<T> R_from_quat(const Quaternion<T>& quat) {
    // 0 bounds checking
    // Identity quaternion

    Quaternion<T> unit_quat = unit(quat);

    T w = unit_quat[0][0];
    T i = unit_quat[1][0];
    T j = unit_quat[2][0];
    T k = unit_quat[3][0];

    T R00 = 1 - 2*(j*j + k*k);
    T R01 = 2 * (i*j - k*w);
    T R02 = 2 * (i*k + j*w);

    T R10 = 2 * (i*j + k*w);
    T R11 = 1 - 2 * (i*i + k*k);
    T R12 = 2 * (j*k - i*w);

    T R20 = 2 * (i*k - j*w);
    T R21 = 2 * (j*k + i*w);
    T R22 = 1 - 2*(i*i + j*j);

    return {{{R00, R01, R02},
            {R10, R11, R12},
            {R20, R21, R22}}};
}

// Quat from R
template <typename T>
Quaternion<T> quat_from_R(const Matrix3<T>& R) {
    Matrix3<T> R_T = transpose(R);

    Quaternion<T> q;
    T t;
    // Insomniac games formula, but taking transpose 
    // because they use scaler last convection
    if (R_T[2][2] < 0) {
        if (R_T[0][0] > R_T[1][1]) {
            t = 1 + R_T[0][0] - R_T[1][1] - R_T[2][2];
            q = {R_T[1][2] - R_T[2][1], t, R_T[0][1] + R_T[1][0], R_T[2][0] + R_T[0][2]};  
        } 
        else {
            t = 1 - R_T[0][0] + R_T[1][1] - R_T[2][2];
            q =  {R_T[2][0] - R_T[0][2], R_T[0][1] + R_T[1][0], t, R_T[1][2] + R_T[2][1]};
        }
    }
    else {
        if (R_T[0][0] < -R_T[1][1]) {
            t = 1 - R_T[0][0] - R_T[1][1] + R_T[2][2];
            q = {R_T[0][1] - R_T[1][0], R_T[2][0] + R_T[0][2], R_T[1][2] + R_T[2][1], t};
        }
        else {
            t = 1 + R_T[0][0] + R_T[1][1] + R_T[2][2];
            q = {t, R_T[1][2] - R_T[2][1], R_T[2][0] - R_T[0][2], R_T[0][1] - R_T[1][0]};
        }
    }
    
    
    T scale = 0.5 / sqrt(abs(t));
    for (auto& val : q) {
        val[0] *= scale;
    }

    // Flip if first element is negative
    if (q[0][0] < 0) {
        for (auto& val : q) {
            val[0] *= -1;
        }
    }

    return q;
}

////////////////////// Quat Operations //////////////////////
// Multiply
template <typename T>
Quaternion<T> quat_mult(const Quaternion<T> quat1, const Quaternion<T> quat2) {
    T w1 = quat1[0][0];
    T x1 = quat1[1][0];
    T y1 = quat1[2][0];
    T z1 = quat1[3][0];

    T w2 = quat2[0][0];
    T x2 = quat2[1][0];
    T y2 = quat2[2][0];
    T z2 = quat2[3][0];

    Quaternion<T> result;

    result[0][0] = w1*w2 - x1*x2 - y1*y2 - z1*z2;
    result[1][0] = w1*x2 + x1*w2 + y1*z2 - z1*y2;
    result[2][0] = w1*y2 - x1*z2 + y1*w2 + z1*x2;
    result[3][0] = w1*z2 + x1*y2 - y1*x2 + z1*w2;

    return result;
}

// Inverse flips i, j, k
template <typename T>
Quaternion<T> inv(const Quaternion<T>& quat) {
    Quaternion<T> result = quat;
    result[1][0] *= -1;
    result[2][0] *= -1;
    result[3][0] *= -1;

    return result;
}

////////////////////// Operations //////////////////////

// Apply to vector
template <typename T>
Vector3<T> quat_apply(const Quaternion<T>& quat, const Vector3<T>& vec) {
    Quaternion<T> intermediate = quat_mult(quat, vec_as_quat(vec));
    Quaternion<T> result = quat_mult(intermediate, inv(quat));

    if (abs(result[0][0]) > 0.001) {
        // Quaternion was not normalized
        // raise(bruh)
        // return quat_apply(unit(quat), vec);
        return Vector3<T>();
    }

    return quat_as_vec(result);
}