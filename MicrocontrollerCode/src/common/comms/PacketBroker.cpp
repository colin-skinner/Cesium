#include "PacketBroker.h"

#include "packets/SystemStatusTask.h"
#include "packets/ClockTask.h"

namespace Cesium {

size_t PacketBroker::routed_packets = 0;

Topic PacketBroker::route_packet(BasePacket &packet)
{
    Topic topic = (Topic)packet.get_topic();
    switch(topic) {
    case Topic::SYSTEM_STATUS:
        DEBUG("Received System Status Packet - ");
        SystemStatusTask::route_packet(packet);
        break;

    case Topic::CLOCK:
        DEBUG("Received Clock Packet - ");
        ClockTask::route_packet(packet);
        break;

    case Topic::TEST_ROCKETS:
        DEBUG("Received Test Rocket Packet - ");
        ClockTask::route_packet(packet);
        break;


    default:
        DEBUGLN("Did not route packet");
        SystemStatusTask::send_nack("Topic NotImplemented");       
    }
    

    routed_packets++;

    return topic;
    
}

}