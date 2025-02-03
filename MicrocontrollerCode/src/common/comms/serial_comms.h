#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"


namespace Cesium {

enum CommsInterface {
    RADIO,
    SERIAL_UART,
    CAN_BUS,
    MOCK_UART
};

class MockSerial {
public:
    std::vector<uint8_t> buffer;
    BasePacket packet;

    MockSerial() : buffer{}, packet{} {}

    void write(std::vector<uint8_t>& buf) {
        buffer.assign(buf.begin(), buf.end() - 1);

        BasePacket::depacketize(buffer, packet);
    }

    void write(String str) {
        buffer.assign(str.c_str(), str.c_str() + str.length());

        BasePacket::depacketize(buffer, packet);
    }

    
};

class SerialComms {

private:
    static MockSerial* mock_port;
public:
    // TODO: figure out Radio, CAN, UART, etc.
    SerialComms();
    static void emit(std::vector<uint8_t> vec, CommsInterface interface);
    static void emit(String str, CommsInterface interface);

    static void emit_packet(BasePacket& packet, CommsInterface interface);

    static void process_uart();

    static void set_mock_port(MockSerial* port) {mock_port = port;}

};

}