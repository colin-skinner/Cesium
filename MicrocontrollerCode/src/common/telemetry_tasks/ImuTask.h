#pragma once

#include <Arduino.h>
#include "../globals.h"
#include "../comms/packet.h"
#include "../comms/packet_schema.h"

#include "../drivers/sensor_bases/AccelerometerBase.h"
#include "../drivers/sensor_bases/GyroscopeBase.h"
#include "../drivers/sensor_bases/MagnetometerBase.h"
#include <vector>

namespace Cesium {

class PacketBroker; // Forward definition

class ImuTask {
    
private:
    static PacketBroker* broker;
    static size_t routed_packets;

    static std::vector<Sensor::AccelerometerBase*> accels;
    static std::vector<Sensor::GyroscopeBase*> gyros;
    static std::vector<Sensor::MagnetometerBase*> mags;

    static bool check_ranges(std::vector<uint8_t>& data);
public:
    
    static inline void add_accel(Sensor::AccelerometerBase* accel) {accels.push_back(accel);}
    static inline void add_gyro(Sensor::GyroscopeBase* gyro) {gyros.push_back(gyro);}
    static inline void add_mag(Sensor::MagnetometerBase* mag) {mags.push_back(mag);}

    inline static void assign_broker(PacketBroker* broker) {ImuTask::broker = broker;};

    // Returns ImuCMD for unit_testing verification
    static ImuCMD route_packet(BasePacket& packet);

    static bool create_telem_packet(std::vector<uint8_t> data);

    static bool send_telem_response(const char* message);
};

}