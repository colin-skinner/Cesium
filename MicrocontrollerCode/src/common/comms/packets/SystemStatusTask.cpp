#include "SystemStatusTask.h"
#include "../PacketBroker.h"


#include "../serial_comms.h"

namespace Cesium {


SystemStatusCMD SystemStatusTask::route_packet(BasePacket &packet)
{
    SystemStatusCMD command = (SystemStatusCMD)packet.get_command();
    switch(command) {
    case SystemStatusCMD::REQUEST_ACK:
        DEBUGLN("REQUEST_ACK");
        send_ack();
        break;

    case SystemStatusCMD::ACK: // No further action
        DEBUGLN("ACK");
        break;

    case SystemStatusCMD::NACK: // No further action
        DEBUGLN("NACK");
        break;

    case SystemStatusCMD::RESET: // TODO
        DEBUGLN("RESET");
        send_not_implemented("SYSTEM_STATUS::RESET");
        break;

    case SystemStatusCMD::RESET_STATS: // TODO
        DEBUGLN("RESET_STATS");
        send_not_implemented("SYSTEM_STATUS::RESET_STATS");
        break;

    case SystemStatusCMD::MCU_STATS: // TODO
        DEBUGLN("MCU_STATS");
        send_not_implemented("SYSTEM_STATUS::MCU_STATS");
        break;

    case SystemStatusCMD::SYSTEM_UPDATE: // TODO
        DEBUGLN("SYSTEM_UPDATE");
        send_not_implemented("SYSTEM_STATUS::SYSTEM_UPDATE");
        break;

    case SystemStatusCMD::SUM:
    DEBUGLN("SUM");
        send_sum(packet);
        break;

    default:
        DEBUGLN("Did not finish routing packet");
        SystemStatusTask::send_nack("BAD COMMAND");
    }

    return command;
}

void SystemStatusTask::send_ack(const char* message)
{
    BasePacket ACK_PACKET;
    std::vector<uint8_t> data(message, message + strlen(message));
    ACK_PACKET.configure(0, 1, data);
    ACK_PACKET.packetize();

    SerialComms::emit_packet(ACK_PACKET);
    DEBUGLN("Emitted ACK Packet");
}

void SystemStatusTask::send_nack(const char* message)
{
    BasePacket packet;
    std::vector<uint8_t> data(message, message + strlen(message));
    packet.configure((size_t)Topic::SYSTEM_STATUS, (size_t)SystemStatusCMD::NACK, data);
    packet.packetize();

    
    SerialComms::emit_packet(packet);
    DEBUGLN("Emitted NACK Packet");
}

void SystemStatusTask::send_sum(BasePacket &packet)
{
    BasePacket response;
    uint8_t sum = packet.get_data()[0] + packet.get_data()[1];
    std::vector<uint8_t> data = {sum};

    response.configure((size_t)Topic::SYSTEM_STATUS, (size_t)SystemStatusCMD::SUM, data);
    response.packetize();

    
    SerialComms::emit_packet(response);
    DEBUGLN("Emitted SUM Packet");
}

void SystemStatusTask::send_not_implemented(const char* message) {
    BasePacket packet;
    std::vector<uint8_t> data(message, message + strlen(message));
    packet.configure((size_t)Topic::SYSTEM_STATUS, (size_t)SystemStatusCMD::NOT_IMPLEMENTED, data);
    packet.packetize();

    
    SerialComms::emit_packet(packet);
    DEBUGLN("Emitted NotImplementedl Packet");
}


}


