#include <unity.h>
#include <Arduino.h>

#include "common/comms/PacketBroker.h"

#include "common/comms/packets/SystemStatusTask.h"

#include "common/comms/packet_schema.h"
#include <vector>

using namespace std;
using namespace Cesium;

void test_00_packet_routed(SystemStatusCMD command) {
    BasePacket packet;
    std::vector<uint8_t> data{1,2};
    packet.configure((size_t) Topic::SYSTEM_STATUS, (size_t) command, data);
    packet.packetize();

    TEST_ASSERT_TRUE(PacketBroker::route_packet(packet) == Topic::SYSTEM_STATUS);
    TEST_ASSERT_TRUE(SystemStatusTask::route_packet(packet) == command);
}


////////////////////////////////////////////////////////////
//                    Packet Routing                      //
////////////////////////////////////////////////////////////

void test_00_request_ack() {
    test_00_packet_routed(SystemStatusCMD::REQUEST_ACK);
}

void test_00_ack() {
    test_00_packet_routed(SystemStatusCMD::ACK);
}

void test_00_nack() {
    test_00_packet_routed(SystemStatusCMD::NACK);
}

void test_00_reset() {
    test_00_packet_routed(SystemStatusCMD::RESET);
}

void test_00_reset_stats() {
    test_00_packet_routed(SystemStatusCMD::RESET_STATS);
}

void test_00_mcu_stats() {
    test_00_packet_routed(SystemStatusCMD::MCU_STATS);
}

void test_00_sys_update() {
    test_00_packet_routed(SystemStatusCMD::SYSTEM_UPDATE);
}


void test_00_sum() {
    test_00_packet_routed(SystemStatusCMD::SUM);
}

void run_all_system_status_tests() {
    RUN_TEST(test_00_request_ack);
    RUN_TEST(test_00_ack);
    RUN_TEST(test_00_nack);
    RUN_TEST(test_00_reset);
    RUN_TEST(test_00_reset_stats);
    RUN_TEST(test_00_mcu_stats);
    RUN_TEST(test_00_sys_update);
    RUN_TEST(test_00_sum);
}
