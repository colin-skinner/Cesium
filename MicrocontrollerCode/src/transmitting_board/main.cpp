
#include <Arduino.h>
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

// Quaternion<float> ICM2Body;
// Quaternion<float> BMI2Body;

float a_x, a_y, a_z;
float w_x, w_y, w_z = 0;

void setup() {
    Serial.begin(115200);
    
    // Attaching Filesystem to FilesystemTask
    filesystem.begin(true);
    FilesystemTask::attach_filesystem(&filesystem);

    // Adding IMUs to ImuTask
    // ImuTask::add_accel(&imu1);
    // ImuTask::add_accel(&imu2);
    // ImuTask::add_gyro(&imu1);
    // ImuTask::add_gyro(&imu2);
    // ImuTask::add_mag(&imu2);

    // Adding sensors to GNC task

    // Setting GNC task order

    init_cs_pins();

    init_interfaces();

    
    
    

    // Matrix3<float> R_ICM2Body = {{{0,1,0}, {0,0,-1}, {-1,0,0}}};
    // Matrix3<float> R_ICM2Body = {{{0,0,-1}, {1,0,0}, {0,-1,0}}};
    // Matrix3<float> R_ICM2Body = {{{0,1,0}, {0,0,1}, {1,0,0}}};
    // Matrix3<float> R_ICM2Body = {{{0,0,1}, {1,0,0}, {0,1,0}}};
    // Matrix3<float> R_BMI2Body = {{{0,0,-1}, {1,0,0}, {0,-1,0}}};
    // ICM2Body = quat_from_R(R_ICM2Body);
    // BMI2Body = quat_from_R(R_BMI2Body);

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
    uint8_t float_array[8] = {0};
    int id = 0;
    can_bus.receive(id, float_array);

    // Serial.println(float_array[4]);

    if (id == 2) {
        a_x = *(float*)(float_array);
        w_x = *(float*)(float_array + 4);
    }
    else if (id == 3) {
        a_y = *(float*)(float_array);
        w_y = *(float*)(float_array + 4);
    }
    if (id == 4) {
        a_z = *(float*)(float_array);
        w_z = *(float*)(float_array + 4);
    }
    else {
        return;
    }
    

    Serial.print(String(a_x) + " ");
    Serial.print(String(a_y) + " ");
    Serial.print(String(a_z) + " ");
    Serial.print(String(w_x) + " ");
    Serial.print(String(w_x) + " ");
    Serial.print(String(w_x) + " ");
    Serial.println();

    delay(10);
}



// Config file

/*

Config
- Pinouts
- Module/Tasks
- Sensors with their pinouts
- Algorithms with their sensors
*/