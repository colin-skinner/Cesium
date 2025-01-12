#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"


namespace Cesium {
class SerialComms {

private:

public:

    SerialComms();
    static void emit(std::vector<uint8_t> vec);
    static void emit(String str);

    static void emit_packet(BasePacket& packet);

    void process_uart();

};

}