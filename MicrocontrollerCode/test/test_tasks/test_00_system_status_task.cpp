#include <unity.h>
#include <Arduino.h>

#include "common/comms/PacketBroker.h"

#include "common/comms/packets/SystemStatusTask.h"

#include "common/comms/packet_schema.h"
#include <vector>


using namespace std;
using namespace Cesium;

enum class TestEnum {
    VALUE1 = 0,
    VALUE2 = 1,
    VALUE3 = 2
};


void test_00_packet_routed(pair<SystemStatusCMD, vector<uint8_t> > command) {
    BasePacket packet;

    packet.configure((size_t) Topic::SYSTEM_STATUS, (size_t) command.first, command.second);
    packet.packetize();

    TEST_ASSERT_TRUE(PacketBroker::route_packet(packet) == Topic::SYSTEM_STATUS);
    TEST_ASSERT_TRUE(SystemStatusTask::route_packet(packet) == command.first);
}


////////////////////////////////////////////////////////////
//                    Packet Routing                      //
////////////////////////////////////////////////////////////

void test_all_system_routing() {

    vector<std::pair<SystemStatusCMD, std::vector<uint8_t>>> commands = {
        {SystemStatusCMD::REQUEST_ACK, {}},
        {SystemStatusCMD::ACK, {}},
        {SystemStatusCMD::NACK, {}},
        {SystemStatusCMD::RESET, {}},
        {SystemStatusCMD::RESET_STATS, {}},
        {SystemStatusCMD::MCU_STATS, {}},
        {SystemStatusCMD::SYSTEM_UPDATE, {}},
        {SystemStatusCMD::SUM, {1,2}},
        {SystemStatusCMD::NOT_IMPLEMENTED, {}}
    };
    for (auto command : commands) {
        test_00_packet_routed(command);
    }
}


void run_all_system_status_tests() {
    RUN_TEST(test_all_system_routing);
}
