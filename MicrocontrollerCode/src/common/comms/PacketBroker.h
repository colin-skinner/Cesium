#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"
#include "packet_schema.h"


namespace Cesium {

class PacketBroker {

private:
    static size_t routed_packets;
public:

    // PacketBrokerClass() : routed_packets(0) {}

    static Topic route_packet(BasePacket& packet);

};


}