/**
 * Plan
 * 1. Because this is all hard coded, during setup, just add sensor objects to be read from into the buffers
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

#include "../drivers/Bmp388.h"
#include "../drivers/Ms5607.h"
#include "../drivers/Icm20948.h"
#include "../drivers/Bmi323.h"
#include "../drivers/UBloxGps.h"

#include <vector>

using Frame = std::array<uint8_t, 8>;
using Packet = std::array<uint8_t, 20>;



namespace Cesium {



class PacketBroker; // Forward definition

// // TODO: make tests
class TestRocketTask {
    
protected:
    static PacketBroker* _broker;
    static size_t routed_packets;
    static CanBus* _can_instance;

    static Sensor::Bmp388* _altimeter1;
    static Sensor::Ms5607* _altimeter2;
    static Sensor::Icm20948* _imu2;
    static Sensor::Bmi323* _imu1;
    static Sensor::UBloxGps* _gps;


    static uint32_t packet_spacing_ms;

    // Array of seven 8-byte frames
    static std::array<Frame, 7> frames;

    // Array of three 20-byte packets
    static std::array<Packet, 3> packets;
    
public:

    static void attach_sensor_objs(Sensor::Bmp388* altimeter1,
        Sensor::Ms5607* altimeter2,
        Sensor::Icm20948* imu2,
        Sensor::Bmi323* imu1,
        Sensor::UBloxGps* gps);

    inline static void attach_CAN_obj(CanBus* can_instance) {TestRocketTask::_can_instance = can_instance;}

    static bool process_incoming_can();
    // inline static void attach_radio_obj(Radio* can_instance) {_can_instance = can_instance;}

    // // Send radio packets by taking data from the buffers
    // static bool send_packets(); 

    // Make and send frames by taking data from the buffers
    // All frames are hard coded (cope.)
    static bool make_frames(uint8_t frames_to_send); 
    static bool send_frames(bool make_can_frames = true, uint8_t frames_to_send = 0b11111110); 



    // Returns TestRocketCMD for unit_testing verification
    static TestRocketCMD route_packet(BasePacket& packet);

    static Packet& get_packet();

    static bool create_telem_packet(std::vector<uint8_t> data);

    static bool send_telem_response(const char* message);

private:
    static bool make_frame_1();
    static bool make_frame_234();
    static bool make_frame_5();
    static bool make_frame_6();
    static bool make_frame_7();
};

}