#pragma once

#include "../comms/packet.h"
#include "../comms/packet_schema.h"
#include "../comms/serial_comms.h"
#include "../os/filesystem.h"

namespace Cesium {

class PacketBroker; // Forward definition

class FilesystemTask {
    
private:
    static PacketBroker* broker;
    static FileSystem* filesystem_ptr;

public:

    static void attach_filesystem(FileSystem* filesystem) {FilesystemTask::filesystem_ptr = filesystem;};
    
    inline static void assign_broker(PacketBroker* broker) {FilesystemTask::broker = broker;};

    // Returns FilesystemCMD for unit_testing
    static FilesystemCMD route_packet(BasePacket& packet);

    // Returns false if listing directories is false
    static bool downlink_directories(FileSystem* filesystem, const char* path);

    // For testing
    static CommsInterface interface;
};

}