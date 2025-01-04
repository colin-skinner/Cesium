#pragma once

#include "../math/vector.h"
#include "../drivers/icm20948.h"
#include "../gnc/GncBase.h"
#include "../globals.h"
namespace Cesium {
    
class TestImuBase {

    Matrix<float,3,3> imu_to_body = {{{1,0,0},{0,1,0},{0,0,1}}};
    
    Icm20948 icm;
    ImuBase imuBase;

public:
    TestImuBase() {}

    void config() {
        icm.accel.x= 1.0;
        icm.accel.y = 2.0;
        icm.accel.z = 3.0;

        imuBase.config(imu_to_body, (float*)&icm.accel, (float*)&icm.gyro);
    }

    void run() {
        imuBase.run();

        printMatrix(imuBase.accel_body);
        DEBUGLN();

        icm.accel.x += 0.1;
        icm.accel.y += 0.1;
        icm.accel.z += 0.1;
    }

};


}
