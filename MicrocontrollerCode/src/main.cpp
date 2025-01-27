
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/comms/serial_comms.h"
#include "common/globals.h"
#include "common/drivers/Icm20948.h"
#include "common/telem_tasks/ImuTask.h"
// using namespace Cesium::Config;
// using namespace Cesium::File;
using namespace Cesium;
using namespace std;

#define LED 2

#define HSCK 14
#define HMISO 36
#define HMOSI 13
#define IMU_CS 33

SerialComms comms;

Sensor::Icm20948 sensor(IMU_CS, &SPI);

void setup() {
    Serial.begin(115200);

    SPI.begin(HSCK, HMISO, HMOSI, IMU_CS);

    sensor.setup();

    ImuTask::add_accel(&sensor);
    ImuTask::add_gyro(&sensor);
    ImuTask::add_mag(&sensor);


    // for (auto row : yes) {
    //     for (auto val : row) {
    //         Serial.print(String(val) + " ");
    //     }
    //     Serial.print("\n");
    // }
}

void loop() {

    // Serial.println(100);
    // delay(1000);
    if (Serial.available()) {
        uint32_t time = millis();
        digitalWrite(LED, HIGH);
        comms.process_uart();
        // Serial.println(millis() - time);
    }
    else {
        digitalWrite(LED, LOW);
    }
    delay(10);

}
