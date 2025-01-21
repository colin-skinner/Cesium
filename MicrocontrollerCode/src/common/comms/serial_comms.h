#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"


namespace Cesium {

enum CommsInterface {
    RADIO,
    SERIAL_UART,
    CAN_BUS
};

class SerialComms {

private:

public:
    // TODO: figure out Radio, CAN, UART, etc.
    SerialComms();
    static void emit(std::vector<uint8_t> vec, CommsInterface interface);
    static void emit(String str, CommsInterface interface);

    static void emit_packet(BasePacket& packet, CommsInterface interface = SERIAL_UART);

    void process_uart();

};

}