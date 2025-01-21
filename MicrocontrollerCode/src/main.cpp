
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/comms/serial_comms.h"
#include "common/globals.h"
#include <vector>
#include <string>
#include "common/tasks/SystemStatusTask.h"

/* You only need to format LittleFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin

   If you test two partitions, you need to use a custom
   partition.csv file, see in the sketch folder */

//#define TWOPART

#define FORMAT_LITTLEFS_IF_FAILED true

using namespace Cesium;
using namespace std;

#define LED 2

SerialComms comms;

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

}

void loop() {

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

    // digitalWrite(LED, HIGH);
    // SystemStatusTask::send_ack();
    // digitalWrite(LED, LOW);
    // delay(1000);

}
