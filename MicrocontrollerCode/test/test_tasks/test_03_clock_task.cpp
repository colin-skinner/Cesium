#include <unity.h>
#include <Arduino.h>

#include "common/comms/PacketBroker.h"

#include "common/telemetry_tasks/ClockTask.h"

#include "common/comms/packet_schema.h"
#include <vector>

using namespace std;
using namespace Cesium;

enum class TestEnum {
    VALUE1 = 0,
    VALUE2 = 1,
    VALUE3 = 2
};


void test_03_packet_routed(pair<ClockCMD, vector<uint8_t> > command) {
    BasePacket packet;
    std::vector<uint8_t> data{1,2};
    packet.configure((size_t) Topic::SYSTEM_STATUS, (size_t) command.first, command.second);
    packet.packetize();

    TEST_ASSERT_EQUAL(Topic::SYSTEM_STATUS, PacketBroker::route_packet(packet));
    TEST_ASSERT_EQUAL(command.first, ClockTask::route_packet(packet));


    String message = "Failed with command = " + (size_t)command.first;
    // TEST_FAIL_MESSAGE(message.c_str());
}


////////////////////////////////////////////////////////////
//                    Packet Routing                      //
////////////////////////////////////////////////////////////

void test_all_clock_routing() {

    vector< pair< ClockCMD, vector<uint8_t> > > commands = {
        {ClockCMD::STATUS, {}},
        {ClockCMD::DAY_SYNC, {}},
        {ClockCMD::JUMP_CLOCK_TELEM, {}},
        {ClockCMD::JUMP_CLOCK_GPS, {}}
    };

    TEST_ASSERT_EQUAL((size_t)commands[1].first, 1);

    // for (auto command : commands) {
    //     test_03_packet_routed(command);
    // }
}


void run_all_clock_tests() {
    RUN_TEST(test_all_clock_routing);
}
