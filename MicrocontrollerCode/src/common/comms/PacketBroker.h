#pragma once

#include <Arduino.h>
#include <vector>
#include "packet.h"
#include "packet_schema.h"

#include "packets/SystemStatusTask.h"

namespace Cesium {

class PacketBroker {

private:
    static uint16_t routed_packets;
public:

    // PacketBrokerClass() : routed_packets(0) {}

    static Topic route_packet(BasePacket& packet);

};


}