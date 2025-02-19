/**
 * Plan
 * 1. During setup, assign pointers to places in a dedicated frame
 * 2. In runtime, gets data frames over CAN every 30ms
 */

#pragma once

#include <Arduino.h>
#include <array>

#include "../globals.h"
#include "../math/vector.h"
#include "../comms/packet.h"
#include "../comms/packet_schema.h"
#include "../comms/CanBus.h"

#include <vector>

using Frame = std::array<uint8_t, 8>;
using Packet = std::array<uint8_t, 20>;

namespace Cesium {

class PacketBroker; // Forward definition

// TODO: make tests
class TestRocketTask {
    
protected:
    static PacketBroker* broker;
    static size_t routed_packets;
    static CanBus* _can_instance;

    static uint32_t packet_spacing_ms;

    // Functions to get numbers for the frames
    static uint32_t (*get_lat_func)(); static uint32_t (*get_lon_func)();                       // Packet 1
    static Vector3<float> (*get_accel_func)(); static Vector3<float> (*get_w_func)();   // Packet 234
    static float (*get_baro_1_alt_func)(); static float (*get_baro_2_alt_func)();               // Packet 5
    static float (*get_batt_V_func)(); static float (*get_batt_I_func)();                       // Packet 4
    static uint32_t (*get_gps_alt_func)();                                                      // Packet 7
    static uint8_t (*get_SIV_func)();                                                           // "
    static uint8_t (*get_frame_func)();                                                         // "
    static uint8_t (*get_FSM_states_func)();                                                    // "
    static uint8_t (*get_logging_status)();                                                     // "

    // Array of seven 8-byte frames
    static std::array<Frame, 7> frames;

    // Array of three 20-byte packets
    static std::array<Packet, 3> packets;
    
public:

    inline static void attach_CAN_obj(CanBus* can_instance) {TestRocketTask::_can_instance = can_instance;}
    static bool process_can();
    // inline static void attach_radio_obj(Radio* can_instance) {_can_instance = can_instance;}

    // Send packets by taking data from the buffers
    static bool send_packets(); 

    // Make and sned frames by taking data from the buffers
    static bool make_frames(uint8_t frames_to_send); 
    static bool send_frames(bool make_can_frames = true, uint8_t frames_to_send = 0b11111110); 

    // All of these packets are hard coded (cope.)
    
    static void configure_frame_1(uint32_t (*get_lat_func)(), uint32_t (*get_lon_func)());
    static void configure_frame_234(Vector3<float> (*get_accel_func)(), Vector3<float> (*get_w_func)());
    static void configure_frame_5(float (*get_baro_1_alt_func)(), float (*get_baro_2_alt_func)());
    static void configure_frame_6(float (*get_batt_V_func)(), float (*get_batt_I_func)());
    static void configure_frame_7(uint32_t (*get_gps_alt_func)(), 
                                 uint8_t (*get_SIV_func)(), 
                                 uint8_t (*get_frame_func)(), 
                                 uint8_t (*get_FSM_states_func)(),
                                 uint8_t (*get_logging_status)());


    // Returns TestRocketCMD for unit_testing verification
    static TestRocketCMD route_packet(BasePacket& packet);

    static Packet& get_packet();

    // static bool create_telem_packet(std::vector<uint8_t> data);

    // static bool send_telem_response(const char* message);

private:
    static bool make_frame_1();
    static bool make_frame_234();
    static bool make_frame_5();
    static bool make_frame_6();
    static bool make_frame_7();
};

}