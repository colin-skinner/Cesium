
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/comms/serial_comms.h"
#include "common/globals.h"
#include "common/drivers/Icm20948.h"
#include "common/drivers/Ms5607.h"
#include "common/drivers/Bmi323.h"
#include "common/drivers/Bmp388.h"

#include "common/telemetry_tasks/ImuTask.h"

// using namespace Cesium::Config;
// using namespace Cesium::File;
using namespace Cesium;
using namespace std;

#define LED 2

#define HSCK 14
#define HMISO 36
#define HMOSI 13
#define IMU_CS 33

#define VSCK 18
#define VMISO 19
#define VMOSI 23
#define MS5607_CS 2
#define BMP_CS 32
#define BMI_CS 16

SerialComms comms;



// SPIClass* vspi = new SPIClass(VSPI);
// SPIClass* hspi = new SPIClass(HSPI);
// SPIClass hspi(VSPI);
SPIClass vspi(VSPI);

// Sensor objects
Sensor::Ms5607 altimeter2(MS5607_CS, &vspi);
Sensor::Icm20948 imu2(IMU_CS, &vspi);
Sensor::Bmi323 imu1(BMI_CS, &vspi);
// Sensor::Bmp388 altimeter1(BMP_CS, &vspi);

// Sensor::Ms

void setup() {
    Serial.begin(115200);

    pinMode(MS5607_CS, OUTPUT);
    pinMode(IMU_CS, OUTPUT);
    pinMode(16, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(32, OUTPUT);

    digitalWrite(MS5607_CS, HIGH);
    digitalWrite(IMU_CS, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(32, HIGH);
    // hspi->begin(HSCK, HMISO, HMOSI);
    vspi.begin(VSCK, VMISO, VMOSI);
    
    imu1.setup();
    imu2.setup();
    altimeter2.setup();


    // Adding sensors to tasks
    // ImuTask::add_accel(&imu2);
    // ImuTask::add_gyro(&imu2);
    // ImuTask::add_mag(&imu2);




    // for (auto row : yes) {
    //     for (auto val : row) {
    //         Serial.print(String(val) + " ");
    //     }
    //     Serial.print("\n");
    // }
}

void loop() {
    altimeter2.read();
    imu1.read();
    imu2.read();

    // Serial.println(100);
    // delay(1000);
    // if (Serial.available()) {
    //     uint32_t time = millis();
    //     digitalWrite(LED, HIGH);
    //     comms.process_uart();
    //     // Serial.println(millis() - time);
    // }
    // else {
    //     digitalWrite(LED, LOW);
    // }
    // delay(10);

}



// Config file

/*

Config
- Pinouts
- Module/Tasks
- Sensors with their pinouts
- Algorithms with their sensors
*/