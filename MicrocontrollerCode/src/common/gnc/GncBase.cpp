#include "GncBase.h"

namespace Cesium {

ImuBase::ImuBase() : accel{}, gyro{}, imu_to_body{} {}

bool ImuBase::config(Matrix3<float> M_to_B, float *accel_array, float *gyro_array)
{
    imu_to_body = M_to_B;

    accel_ptr = accel_array;
    gyro_ptr = gyro_array;

    return true;
}

bool ImuBase::run()
{
    accel[0] = {accel_ptr[0]};
    accel[1] = {accel_ptr[1]};
    accel[2] = {accel_ptr[2]};

    gyro[0] = {gyro_ptr[0]};
    gyro[1] = {gyro_ptr[1]};
    gyro[2] = {gyro_ptr[2]};

    accel_body = imu_to_body * accel;
    gyro_body = imu_to_body * gyro;

    return true;
}

}