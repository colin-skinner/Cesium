#include "TestRocketTask.h"
#include "SystemStatusTask.h"

#include "../comms/serial_comms.h"
#include "../math/vector.h"

using namespace std;

namespace Cesium {

uint32_t TestRocketTask::packet_spacing_ms = 300;
CanBus* TestRocketTask::_can_instance = nullptr;

uint32_t (*TestRocketTask::get_lat_func)() = nullptr; uint32_t (*TestRocketTask::get_lon_func)() = nullptr;         // Packet 1
Vector3<float> (*TestRocketTask::get_accel_func)() = nullptr; Vector3<float> (*TestRocketTask::get_w_func)() = nullptr;           // Packet 234
float (*TestRocketTask::get_baro_1_alt_func)() = nullptr; float (*TestRocketTask::get_baro_2_alt_func)() = nullptr; // Packet 5
float (*TestRocketTask::get_batt_V_func)() = nullptr; float (*TestRocketTask::get_batt_I_func)() = nullptr;         // Packet 6

// Packet 7
uint32_t (*TestRocketTask::get_gps_alt_func)() = nullptr;
uint8_t (*TestRocketTask::get_SIV_func)() = nullptr;
uint8_t (*TestRocketTask::get_frame_func)() = nullptr;
uint8_t (*TestRocketTask::get_FSM_states_func)() = nullptr;
uint8_t (*TestRocketTask::get_logging_status)() = nullptr;

// Packets
array<Frame, 7> TestRocketTask::frames{};
array<Packet, 3> TestRocketTask::packets{};

TestRocketCMD TestRocketTask::route_packet(BasePacket& packet) {
    TestRocketCMD command = (TestRocketCMD)packet.get_command();
    switch(command) {
    case TestRocketCMD::CONFIGURE:
        DEBUGLN("TR CONFIGURE");
        // Configure based off of data
        break;
    
    case TestRocketCMD::PACKET_A:
        DEBUGLN("TR A");
        // Emit packet A
        break;

    case TestRocketCMD::PACKET_B:
        DEBUGLN("TR B");
        // Emit packet B
        break;

    case TestRocketCMD::PACKET_C:
        DEBUGLN("TR C");
        // Emit packet C
        break;
    
    default:
        DEBUGLN("Did not finish routing packet");
        SystemStatusTask::send_nack("BAD COMMAND");
        return TestRocketCMD(-1);
    }

    return command;
}

bool TestRocketTask::process_can() {
    uint8_t receive_buffer[8] = {0};
    int id = 0;

    while (_can_instance->receive(id, receive_buffer) != 0) {

        switch(id) {
        case 1:
            memcpy(&packets[0] + 12, receive_buffer, 8);    // Lat, lon
            break;
        case 2:
            memcpy(&packets[0], receive_buffer, 4);         // accel_x
            memcpy(&packets[1], receive_buffer + 4, 4);
            break;
        case 3:
            memcpy(&packets[0] + 4, receive_buffer, 4);     // accel_y
            memcpy(&packets[1] + 4, receive_buffer + 4, 4);
            break;
        case 4:
            memcpy(&packets[0] + 8, receive_buffer, 4);     // accel_z
            memcpy(&packets[1] + 8, receive_buffer + 4, 4);
            break;
        case 5:
            memcpy(&packets[2] + 8, receive_buffer, 8);     // baro 1,2 alt_m
            break;
        case 6:
            memcpy(&packets[2], receive_buffer, 8);     // Volt, current
            break;
        case 7:
            memcpy(&packets[1] + 12, receive_buffer, 4);     // GPS alt
            memcpy(&packets[1] + 16, receive_buffer + 4, 1);     // Frame/SIV
            memcpy(&packets[2] + 16, receive_buffer + 5, 1);     // FSM Status
            memcpy(&packets[2] + 17, receive_buffer + 6, 1);     // Logging Status
            break;
        default:
            DEBUGLN("CAN ID not 1-7 inclusive.");
        }

    }

    return true;
}

////////////////////////////////////////////////////////////
//                     Telemetry                          //
////////////////////////////////////////////////////////////

bool TestRocketTask::send_packets()
{
    // https://www.simonv.fr/TypesConvert/?integers for ints
    // https://gregstoll.com/~gregstoll/floattohex/ for floats

    Vector<uint8_t, 20> packet_A = {
        0x3f, 0x80, 0x00, 0x00, // 0.1 m/s2
        0x3f, 0x80, 0x00, 0x00, // 0.1 m/s2
        0xc1, 0x1c, 0xe8, 0x0a, // 9.80665 m/s2
        0x14, 0x4E, 0x9A, 0xE6, // 340695782 deg * 10^-7
        0xB9, 0x65, 0x92, 0xBE  // -1184525635 deg * 10^-7
    }; 

    Vector<uint8_t, 20> packet_B = {
        0xbd, 0xcc, 0xcc, 0xcd, // -0.1 rad/s
        0xbe, 0x4c, 0xcc, 0xcd, // -0.2 rad/s
        0xbe, 0x99, 0x99, 0x9a, // -0.3 rad/s
        0x00, 0x01, 0xC1, 0x38, // 115000 mm
        0b10001000              // Body frame of 2 and SIV is 8
    }; // Frame is upper 2 bits, SIV is lower 6 bits

    Vector<uint8_t, 20> packet_C = {
        0x40, 0xec, 0xcc, 0xcd, // 7.4 V
        0x3e, 0x19, 0x99, 0x9a, // 0.150 A
        0x42, 0xe6, 0x00, 0x00, // 115000 mm
        0x42, 0xe6, 0x00, 0x00, // 115000 mm
        0x41, 0x07              // FSM state of 1, Logging status of 7
    }; 

    memcpy(&packets[0], &packet_A, sizeof(packet_A));
    memcpy(&packets[1], &packet_B, sizeof(packet_B));
    memcpy(&packets[2], &packet_C, sizeof(packet_C));

    /*
    // Packet A
    Radio.send_packet((uint8_t*)(&packets[0]), 20);

    // Packet B
    Radio.send_packet((uint8_t*)(&packets[1]), 16);

    // Packet C
    Radio.send_packet((uint8_t*)(&packets[0]), 17);
    
    
    */
                                
    return false;
}

////////////////////////////////////////////////////////////
//                     Make Frames                        //
////////////////////////////////////////////////////////////

bool TestRocketTask::make_frames(uint8_t frames_to_send)
{
    // RETURN_FALSE_IF_FALSE(make_frame_1());
    RETURN_FALSE_IF_FALSE(make_frame_234());
    // RETURN_FALSE_IF_FALSE(make_frame_5());
    // RETURN_FALSE_IF_FALSE(make_frame_6());
    // RETURN_FALSE_IF_FALSE(make_frame_7());

    return true;
}

bool TestRocketTask::send_frames(bool make_can_frames, uint8_t frames_to_send)
{
    // If CAN instance is not initialized, return
    if (_can_instance == nullptr) {
        Serial.println("CAN Instance not initialized");
        return false;
    }

    // Makes frames
    if (make_can_frames) {
        make_frames(frames_to_send);
    }

    for (uint8_t i = 2; i <= 4; i++) {
        // Serial.println(i);
        _can_instance->transmit(i, (uint8_t*)&frames[i-1], 8);
    }

    return true;
}

void TestRocketTask::configure_frame_1(uint32_t (*get_lat_func)(), uint32_t (*get_lon_func)())
{
    TestRocketTask::get_lat_func = get_lat_func;
    TestRocketTask::get_lon_func = get_lon_func;
}

void TestRocketTask::configure_frame_234(Vector3<float> (*get_accel_func)(), Vector3<float> (*get_w_func)())
{
    TestRocketTask::get_accel_func = get_accel_func;
    TestRocketTask::get_w_func = get_w_func;
}

void TestRocketTask::configure_frame_5(float (*get_baro_1_alt_func)(), float (*get_baro_2_alt_func)())
{
    TestRocketTask::get_baro_1_alt_func = get_baro_1_alt_func;
    TestRocketTask::get_baro_2_alt_func = get_baro_2_alt_func;
}

void TestRocketTask::configure_frame_6(float (*get_batt_V_func)(), float (*get_batt_I_func)())
{
    TestRocketTask::get_batt_V_func = get_batt_V_func;
    TestRocketTask::get_batt_I_func = get_batt_I_func;
}

void TestRocketTask::configure_frame_7(uint32_t (*get_gps_alt_func)(), uint8_t (*get_SIV_func)(), uint8_t (*get_frame_func)(), uint8_t (*get_FSM_states_func)(), uint8_t (*get_logging_status)())
{
    TestRocketTask::get_gps_alt_func = get_gps_alt_func;
    TestRocketTask::get_SIV_func = get_SIV_func;
    TestRocketTask::get_frame_func = get_frame_func;
    TestRocketTask::get_FSM_states_func = get_FSM_states_func;
    TestRocketTask::get_logging_status = get_logging_status;
}

bool TestRocketTask::make_frame_1()
{
    if (get_lat_func == nullptr || get_lon_func == nullptr) {
        DEBUGLN("One or more functions are not specified");
        return false;
    }

    auto lat = get_lat_func();
    auto lon = get_lon_func();

    memcpy(&(frames[0][0]), &lat, sizeof(lat));
    memcpy(&(frames[0][4]), &lon, sizeof(lon));

    return true;
}

bool TestRocketTask::make_frame_234()
{

    if (get_accel_func == nullptr || get_w_func == nullptr) {
        DEBUGLN("One or more functions are not specified");
        return false;
    }

    Vector3<float> accel = get_accel_func();
    Vector3<float> w = get_w_func();

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[1][0]), &accel[0], sizeof(accel[0]));
    memcpy(&(frames[1][4]), &w[0], sizeof(w[0]));

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[2][0]), &accel[1], sizeof(accel[1]));
    memcpy(&(frames[2][4]), &w[1], sizeof(w[1]));

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[3][0]), &accel[2], sizeof(accel[2]));
    memcpy(&(frames[3][4]), &w[2], sizeof(w[2]));

    return true;
}

bool TestRocketTask::make_frame_5()
{
    if (get_baro_1_alt_func == nullptr || get_baro_2_alt_func == nullptr) {
        DEBUGLN("One or more functions are not specified");
        return false;
    }

    auto alt_1 = get_baro_1_alt_func();
    auto alt_2 = get_baro_2_alt_func();

    memcpy(&(frames[4][0]), &alt_1, sizeof(alt_1));
    memcpy(&(frames[4][4]), &alt_2, sizeof(alt_2));

    return true;
}

bool TestRocketTask::make_frame_6()
{
    if (get_batt_V_func == nullptr || get_batt_I_func == nullptr) {
        DEBUGLN("One or more functions are not specified");
        return false;
    }

    auto V = get_batt_V_func();
    auto I = get_batt_I_func();

    memcpy(&(frames[5][0]), &V, sizeof(V));
    memcpy(&(frames[5][4]), &I, sizeof(I));

    return true;
}

bool TestRocketTask::make_frame_7()
{
    if (get_gps_alt_func == nullptr 
        || get_SIV_func == nullptr
        || get_frame_func == nullptr
        || get_FSM_states_func == nullptr
        || get_logging_status == nullptr) {

        DEBUGLN("One or more functions are not specified");
        return false;
    }

    auto alt = get_gps_alt_func();
    auto SIV = get_SIV_func();
    auto frame = get_frame_func();
    auto FSM = get_FSM_states_func();
    auto log = get_logging_status();

    uint8_t frame_SIV = ( (frame << 6) && 0b11000000) | (SIV && 0b00111111);
    

    memcpy(&(frames[6][0]), &alt, sizeof(alt));
    memcpy(&(frames[6][4]), &frame_SIV, sizeof(frame_SIV));
    memcpy(&(frames[6][6]), &FSM, sizeof(FSM));
    memcpy(&(frames[6][7]), &log, sizeof(log));

    return true;
}





}