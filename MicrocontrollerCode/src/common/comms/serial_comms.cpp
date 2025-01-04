#include "serial_comms.h"
#include "PacketBroker.h" //Importing PacketBroker

#include "packets/SystemStatusTask.h" // ACK
namespace Cesium {
SerialComms::SerialComms() {}

void SerialComms::emit_byte_vector(std::vector<uint8_t> vec) {
    for (auto byte : vec) {
        Serial.write(byte);
    }
    Serial.println(); // NEED to add a line
}

void SerialComms::emit_packet(BasePacket& packet) {
    emit_byte_vector(packet.get_packet());
}

void SerialComms::process_uart() {
    String message = Serial.readStringUntil('\n');
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