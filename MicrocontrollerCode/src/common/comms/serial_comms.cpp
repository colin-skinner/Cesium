#include "serial_comms.h"
#include "PacketBroker.h" //Importing PacketBroker

#include "../telemetry_tasks/SystemStatusTask.h" // ACK
namespace Cesium {

MockSerial* SerialComms::mock_port = nullptr;

SerialComms::SerialComms() {}

void SerialComms::emit(std::vector<uint8_t> vec, CommsInterface interface) {

    switch(interface) {

    case SERIAL_UART:
        for (auto byte : vec) {
            Serial.write(byte);
        }
        break;
    case MOCK_UART:
        mock_port->write(vec);
        break;
    }
    // Serial.write('\0'); // NEED to add a line
}

void SerialComms::emit(String str, CommsInterface interface)
{
    switch(interface) {

    case SERIAL_UART:
        Serial.write(str.c_str());
        break;
    case MOCK_UART:
        mock_port->write(str);
    }
    
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

        
    }
}

}