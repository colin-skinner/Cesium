#include "serial_comms.h"
#include "PacketBroker.h" //Importing PacketBroker

#include "../tasks/SystemStatusTask.h" // ACK
namespace Cesium {
SerialComms::SerialComms() {}

void SerialComms::emit(std::vector<uint8_t> vec, CommsInterface interface) {
    for (auto byte : vec) {
        Serial.write(byte);
    }
    // Serial.write('\0'); // NEED to add a line
}

void SerialComms::emit(String str, CommsInterface interface)
{
    Serial.write(str.c_str());
}

void SerialComms::emit_packet(BasePacket& packet, CommsInterface interface) {
    emit(packet.get_packet(), interface);
}

void SerialComms::process_uart() {
    String message = Serial.readStringUntil(0x00);

    for (auto char_str : message) {
        DEBUG(char_str, HEX);
        DEBUG(" ");
    }
    DEBUGLN();
    std::vector<uint8_t> received_data;
    received_data.assign(message.c_str(), message.c_str() + message.length()); // Copies C string

    // Attempt to depacketize
    BasePacket result;
    if (BasePacket::depacketize(received_data, result) != BASE_PACKET_NO_ERR) {
        // NACK if fail
        // SystemStatusTask::send_nack();
        return;
    }
    else {
        // ACK if success
        SystemStatusTask::send_ack();

        PacketBroker::route_packet(result);

        

        // DEBUG("Received packet: {");
        // DEBUG("Topic ID: " + String(result.get_topic(), HEX));
        // DEBUG(", Command ID: " + String(result.get_command(), HEX));
        // DEBUG(", Millistamp: " + String(result.get_millistamp(), HEX));
        // DEBUG(", Data Length: " + String(result.get_data_length(), HEX));
        // DEBUG(", CRC: " + String(result.get_crc(), HEX));
        // std::string data_str(result.get_data().begin(), result.get_data().end());
        // DEBUG(", Data: " + String(data_str.c_str()));
        // DEBUGLN("}");

        // for (auto byte : result.get_packet()) {
        //     Serial.write(byte);
        // }
        // Serial.println();
        
    }
}

}