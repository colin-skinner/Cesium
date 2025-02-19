
#include <Arduino.h>
#include <functional>
#include "../common/comms/packet.h"
#include "../common/comms/serial_comms.h"


#include "../common/telemetry_tasks/ImuTask.h"
#include "../common/telemetry_tasks/FilesystemTask.h"
#include "../common/telemetry_tasks/TestRocketTask.h"



#include "../common/math/quaternion.h"

#include "HAL.h"
// using namespace Cesium::Config;
// using namespace Cesium::File;
using namespace Cesium;
using namespace std;

Quaternion<float> ICM2Body;
Quaternion<float> BMI2Body;

void setup() {
    Serial.begin(115200);
    
    // Attaching Filesystem to FilesystemTask
    filesystem.begin(true);
    FilesystemTask::attach_filesystem(&filesystem);

    // Adding IMUs to ImuTask
    ImuTask::add_accel(&imu1);
    ImuTask::add_accel(&imu2);
    ImuTask::add_gyro(&imu1);
    ImuTask::add_gyro(&imu2);
    ImuTask::add_mag(&imu2);

    // Adding sensors to GNC task

    // Setting GNC task order

    init_cs_pins();

    init_interfaces();

    init_sensors();
    
    
    

    // Matrix3<float> R_ICM2Body = {{{0,1,0}, {0,0,-1}, {-1,0,0}}};
    // Matrix3<float> R_ICM2Body = {{{0,0,-1}, {1,0,0}, {0,-1,0}}};
    // Matrix3<float> R_ICM2Body = {{{0,1,0}, {0,0,1}, {1,0,0}}};
    Matrix3<float> R_ICM2Body = {{{0,0,1}, {1,0,0}, {0,1,0}}};
    Matrix3<float> R_BMI2Body = {{{0,0,-1}, {1,0,0}, {0,-1,0}}};
    ICM2Body = quat_from_R(R_ICM2Body);
    BMI2Body = quat_from_R(R_BMI2Body);

    // TestRocketTask::configure_frame_234(
    //     []() { return imu2.get_accel_mps2(); } ,
    //     []() { return imu2.get_w_rps(); }
    // );
    // TestRocketTask::attach_CAN_obj(&can_bus);

}

void loop() {

    // if (Serial.available()) {
    //     uint32_t time = millis();
    //     digitalWrite(LED, HIGH);
    //     SerialComms::process_uart();
    //     // Serial.println(millis() - time);
    // }
    // else {
    //     digitalWrite(LED, LOW);
    // }
    // delay(10);

    altimeter2.read();
    imu1.read();
    imu2.read();

    auto icm_result = quat_apply(ICM2Body, imu2.get_accel_mps2());
    auto bmi_result = quat_apply(BMI2Body, imu1.get_accel_mps2());

    Serial.println(altimeter2.get_altitude_m());
    Serial.println(imu1.get_accel_mps2()[0][0]);
    Serial.println(imu2.get_accel_mps2()[0][0]);
    // auto result = quat_apply(quat_from_axis_rot<float>(90, {{1,0,0}}), imu2.get_accel_mps2());
    // Serial.print(String(icm_result[0][0],4) + " " + String(icm_result[0][1],4) + " " + String(icm_result[0][2],4) + ",");
    // Serial.println(String(bmi_result[0][0],4) + "," + String(bmi_result[0][1],4) + "," + String(bmi_result[0][2],4));

    // auto mat = imu2.get_B_uT();

    // TestRocketTask::configure_frame_1(imu2.get_accel_body_mps2()[0], )
    // uint8_t seven[] = {1,2,3,4,5};
    // float NUMBER = 40;
    // Serial.println("Can success: " + String(can_bus.transmit(1, (uint8_t*)(&NUMBER), 4)));
    

    // Serial.println(String(mat[0][0],7) + "," + String(mat[0][1],7) + "," + String(mat[0][2],7));
    // TestRocketTask::send_frames();
    delay(100);
}



// Config file

/*

Config
- Pinouts
- Module/Tasks
- Sensors with their pinouts
- Algorithms with their sensors
*/