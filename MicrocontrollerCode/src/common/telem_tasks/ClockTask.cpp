#include "ClockTask.h"
#include "SystemStatusTask.h"
#include "../comms/serial_comms.h"

using namespace std;

namespace Cesium {

size_t ClockTask::routed_packets = 0;

ClockCMD ClockTask::route_packet(BasePacket &packet)
{
    ClockCMD command = (ClockCMD)packet.get_command();
    switch(command) {
    case ClockCMD::STATUS:
        DEBUGLN("STATUS");
        SystemStatusTask::send_not_implemented("CLOCK::STATUS");
        break;

    case ClockCMD::DAY_SYNC: // No further action
        DEBUGLN("DAY_SYNC");
        SystemStatusTask::send_not_implemented("CLOCK::DAY_SYNC");
        break;

    case ClockCMD::JUMP_CLOCK_TELEM: // No further action
        DEBUGLN("JUMP_CLOCK_TELEM");
        SystemStatusTask::send_ack("CLOCK::JUMP_CLOCK_TELEM");
        jump_clock_telem(packet);
        break;

    case ClockCMD::JUMP_CLOCK_GPS: // TODO
        DEBUGLN("JUMP_CLOCK_GPS");
        SystemStatusTask::send_not_implemented("CLOCK::JUMP_CLOCK_GPS");
        break;

    default:
        DEBUGLN("Did not finish routing packet");
        SystemStatusTask::send_nack("BAD COMMAND");
        return ClockCMD(-1);
    }

    return command;
}

bool ClockTask::jump_clock_telem(BasePacket &packet)
{
    vector<uint8_t> data = packet.get_data();

    // uint8_t, uint8_t, uint16_t
    // day      month    year
    size_t data_size = data.size();

    // Verifies data length
    if (data_size != 4) {
        String err = "Data size '" + String(data_size) + "' when 4 expected";
        DEBUGLN(err);
        send_jump_clock_response(err);
        return false;
    }

    uint8_t day = data[0];
    uint8_t month = data[1];
    uint16_t year = data[2];

    // Set clock of ESP32
    RETURN_FALSE_IF_FALSE(Clock::jump_clock(packet.get_millistamp(), day, month, year));

    send_jump_clock_response("success");
    
    return true;
}

void ClockTask::send_jump_clock_response(String message)
{
    vector<uint8_t> data(message.c_str(), message.c_str() + message.length());

    BasePacket packet;
    packet.configure((int)Topic::CLOCK, (int)ClockCMD::JUMP_CLOCK_TELEM, data);
    packet.packetize();

    SerialComms::emit_packet(packet);
}

}