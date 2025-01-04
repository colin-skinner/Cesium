#pragma once

#include "../packet.h"

namespace Cesium {

class PacketBroker; // Forward definition

class SystemStatusTask {
    
private:
    static PacketBroker* broker;

public:
    
    inline static void assign_broker(PacketBroker* broker) {SystemStatusTask::broker = broker;};

    // Returns SystemStatusCMD for unit_testing to verify packet routing for commands that don't do anything (ACK, NACK)
    static SystemStatusCMD route_packet(BasePacket& packet);

    static void send_ack(const char* message = "ACK");
    static void send_nack(const char* message = "NACK");
    

    static void send_sum(BasePacket& packet);

    static void send_not_implemented(const char* message = "");

    

};

}