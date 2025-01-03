
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/globals.h"
#include <vector>
#include <string>

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

BasePacket result;
vector<uint8_t> received_data;


void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

}

void loop() {

    if (Serial.available()) {
        digitalWrite(LED, HIGH);
        // delay(500);
        // digitalWrite(LED, LOW);
        // DEBUG("Received bit: ");
        // DEBUGLN(Serial.read());
        // DEBUG("Received message: ");
        String message = Serial.readStringUntil('\n');
        received_data.assign(message.c_str(), message.c_str() + message.length());

        if (BasePacket::depacketize(received_data, result) != BASE_PACKET_NO_ERR) {
            DEBUGLN("Invalid Packet");
        }
        else {
            DEBUG("Received packet: {");
            DEBUG("Topic ID: " + String(result.get_topic(), HEX));
            DEBUG(", Command ID: " + String(result.get_command(), HEX));
            DEBUG(", Millistamp: " + String(result.get_millistamp(), HEX));
            DEBUG(", Data Length: " + String(result.get_data_length(), HEX));
            DEBUG(", CRC: " + String(result.get_crc(), HEX));
            std::string data_str(result.get_data().begin(), result.get_data().end());
            DEBUG(", Data: " + String(data_str.c_str()));
            DEBUGLN("}");
        }

        

        // DEBUG("\n");
    }
    else {
        digitalWrite(LED, LOW);
    }
    delay(10);
}
