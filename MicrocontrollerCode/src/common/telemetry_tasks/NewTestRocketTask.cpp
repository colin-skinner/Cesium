#include "NewTestRocketTask.h"
#include "SystemStatusTask.h"

#include "../comms/serial_comms.h"
#include "../math/vector.h"

using namespace std;

namespace Cesium {

uint32_t TestRocketTask::packet_spacing_ms = 300;
CanBus* TestRocketTask::_can_instance = nullptr;

Sensor::Bmp388* TestRocketTask::_altimeter1 = nullptr;
Sensor::Ms5607* TestRocketTask::_altimeter2 = nullptr;
Sensor::Icm20948* TestRocketTask::_imu2 = nullptr;
Sensor::Bmi323* TestRocketTask::_imu1 = nullptr;
Sensor::UBloxGps* TestRocketTask::_gps = nullptr;


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

bool TestRocketTask::process_incoming_can() {
    /*
        Processes incoming CAN messages and memcpys them into the packet buffers
    */

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
//                     Make Frames                        //
////////////////////////////////////////////////////////////

bool TestRocketTask::make_frames(uint8_t frames_to_send)
{
    RETURN_FALSE_IF_FALSE(make_frame_1());
    RETURN_FALSE_IF_FALSE(make_frame_234());
    RETURN_FALSE_IF_FALSE(make_frame_5());
    RETURN_FALSE_IF_FALSE(make_frame_6());
    RETURN_FALSE_IF_FALSE(make_frame_7());

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


bool TestRocketTask::make_frame_1()
{
    auto lat = _gps->get_latitude_deg();
    auto lon = _gps->get_longitude_deg();

    memcpy(&(frames[0][0]), &lat, sizeof(lat));
    memcpy(&(frames[0][4]), &lon, sizeof(lon));

    return true;
}

bool TestRocketTask::make_frame_234()
{

    Vector3<float> accel_1 = _imu1->get_accel_mps2();
    Vector3<float> w_1 = _imu1->get_w_rps();

    Vector3<float> accel_2 = _imu2->get_accel_mps2();
    Vector3<float> w_2 = _imu2->get_w_rps();

    Vector3<float> accel_avg = (accel_1 + accel_2) * (float)(1/2);
    Vector3<float> w_avg = (w_1 + w_2) * (float)(1/2);

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[1][0]), &accel_avg[0], sizeof(accel_avg[0]));
    memcpy(&(frames[1][4]), &w_avg[0], sizeof(w_avg[0]));

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[2][0]), &accel_avg[1], sizeof(accel_avg[1]));
    memcpy(&(frames[2][4]), &w_avg[1], sizeof(w_avg[1]));

    // Packet 1 (a_x, w_x)
    memcpy(&(frames[3][0]), &accel_avg[2], sizeof(accel_avg[2]));
    memcpy(&(frames[3][4]), &w_avg[2], sizeof(w_avg[2]));

    return true;
}

bool TestRocketTask::make_frame_5()
{


    // auto alt_1 = _altimeter1->get(); Figure this one out with barometer
    auto alt_2 = _altimeter2->get_altitude_m();

    memcpy(&(frames[4][0]), 0, sizeof(alt_2));
    // memcpy(&(frames[4][0]), &alt_1, sizeof(alt_1));
    memcpy(&(frames[4][4]), &alt_2, sizeof(alt_2));

    return true;
}

bool TestRocketTask::make_frame_6()
{

    // auto V = get_batt_V_func();
    // auto I = get_batt_I_func();

    // memcpy(&(frames[5][0]), &V, sizeof(V));
    // memcpy(&(frames[5][4]), &I, sizeof(I));
    memcpy(&(frames[5][0]), 0, 8);

    return true;
}

bool TestRocketTask::make_frame_7()
{
    // if (get_gps_alt_func == nullptr 
    //     || get_SIV_func == nullptr
    //     || get_frame_func == nullptr
    //     || get_FSM_states_func == nullptr
    //     || get_logging_status == nullptr) {

    //     DEBUGLN("One or more functions are not specified");
    //     return false;
    // }

    auto alt = _gps->get_altitude_m();
    auto SIV = _gps->get_satellites_in_view();
    auto frame = 0;
    // auto FSM = get_FSM_states_func();
    // auto log = get_logging_status();

    uint8_t frame_SIV = ( (frame << 6) && 0b11000000) | (SIV && 0b00111111);
    

    memcpy(&(frames[6][0]), &alt, sizeof(alt));
    memcpy(&(frames[6][4]), &frame_SIV, sizeof(frame_SIV));
    memcpy(&(frames[6][6]), 0, 1);
    memcpy(&(frames[6][7]), 0, 1);

    return true;
}


////////////////////////////////////////////////////////////
//                       Setup                            //
////////////////////////////////////////////////////////////


void TestRocketTask::attach_sensor_objs(
    Sensor::Bmp388 * altimeter1, 
    Sensor::Ms5607 * altimeter2, 
    Sensor::Icm20948 * imu2, 
    Sensor::Bmi323 * imu1, 
    Sensor::UBloxGps * gps)
{
    _altimeter1 = altimeter1;
    _altimeter2 = altimeter2;
    _imu1 = imu1;
    _imu2 = imu2;
    _gps = gps;
}

}