
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/comms/serial_comms.h"
#include "common/globals.h"
#include "common/drivers/Icm20948.h"
#include "common/drivers/Ms5607.h"
#include "common/drivers/Bmi323.h"
#include "common/drivers/Bmp388.h"

#include "common/telemetry_tasks/ImuTask.h"
#include "common/telemetry_tasks/FilesystemTask.h"

#include "common/os/filesystem.h"
// using namespace Cesium::Config;
// using namespace Cesium::File;
using namespace Cesium;
using namespace std;

FileSystem filesystem;


#define LED 2

void setup() {
    Serial.begin(115200);
    
    filesystem.begin(true);
    FilesystemTask::attach_filesystem(&filesystem);

}

void loop() {

    if (Serial.available()) {
        uint32_t time = millis();
        digitalWrite(LED, HIGH);
        SerialComms::process_uart();
        // Serial.println(millis() - time);
    }
    else {
        digitalWrite(LED, LOW);
    }
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