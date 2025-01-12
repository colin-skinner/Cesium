#pragma once

#include "../packet.h"
#include "../packet_schema.h"
#include "../../clock.h"

namespace Cesium {

class PacketBroker; // Forward definition

class ClockTask {
    
private:
    static PacketBroker* broker;
    static size_t routed_packets;
public:
    
    inline static void assign_broker(PacketBroker* broker) {ClockTask::broker = broker;};

    // Returns ClockCMD for unit_testing verification
    static ClockCMD route_packet(BasePacket& packet);

    static bool jump_clock_telem(BasePacket& packet);


    static void send_jump_clock_response(String message);

};

}