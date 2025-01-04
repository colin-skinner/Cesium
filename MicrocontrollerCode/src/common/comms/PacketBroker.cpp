#include "PacketBroker.h"

namespace Cesium {

uint16_t PacketBroker::routed_packets = 0;

Topic PacketBroker::route_packet(BasePacket &packet)
{
    Topic topic = (Topic)packet.get_topic();
    switch(topic) {
    case Topic::SYSTEM_STATUS:
        DEBUG("Received System Status Packet - ");
        SystemStatusTask::route_packet(packet);
        break;


    default:
        DEBUGLN("Did not route packet");
        SystemStatusTask::send_nack("Topic NotImplemented");       
    }
    

    routed_packets++;

    return topic;
    
}

}