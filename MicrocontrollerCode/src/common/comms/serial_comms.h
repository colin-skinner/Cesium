#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"

namespace Cesium {
BasePacket result;
std::vector<uint8_t> received_data;


void emit_byte_vector(std::vector<uint8_t> vec) {
    for (auto byte : vec) {
        Serial.write(byte);
    }
    Serial.println();
}

void emit_packet(BasePacket& packet) {
    emit_byte_vector(packet.get_packet());
}

void emit_ACK() {
    BasePacket ACK_PACKET;
    std::vector<uint8_t> data = {0x41, 0x43, 0x4B};
    ACK_PACKET.configure(0, 1, data);
    ACK_PACKET.packetize();

    emit_packet(ACK_PACKET);
}


void process_uart() {
    String message = Serial.readStringUntil('\n');
    received_data.assign(message.c_str(), message.c_str() + message.length());

    if (BasePacket::depacketize(received_data, result) != BASE_PACKET_NO_ERR) {
        // DEBUGLN("Invalid Packet");
        return;
    }
    else {
        emit_ACK();
        result.packetize();

        if (result.get_topic() == 0 && result.get_command() == 0) {
            BasePacket packet;
            std::vector<uint8_t> data = {0x41, 0x43, 0x4B};
            packet.configure(0, 1, data);
            packet.packetize();

            // emit_packet(packet);

            
        }

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