#include "FilesystemTask.h"
#include "SystemStatusTask.h"

using namespace std;

namespace Cesium {

CommsInterface FilesystemTask::interface = SERIAL_UART;
FileSystem* FilesystemTask::filesystem_ptr = nullptr;

FilesystemCMD FilesystemTask::route_packet(BasePacket & packet)
{
    FilesystemCMD command = (FilesystemCMD)packet.get_command();
    switch(command) {
    case FilesystemCMD::LIST_DIR: {
        DEBUGLN("LIST_DIR");

        String path((char*)packet.get_data().data(), packet.get_data_length());

        downlink_directories(FilesystemTask::filesystem_ptr, path.c_str());
        
        break;
        }
    case FilesystemCMD::WRITE_FILE:
        DEBUGLN("WRITE_FILE");
        break;

    case FilesystemCMD::READ_FILE:
        DEBUGLN("READ_FILE");
        break;

    case FilesystemCMD::DELETE_FILE:
        DEBUGLN("DELETE_FILE");
        break;

    case FilesystemCMD::FILESYSTEM_STATS:
        DEBUGLN("FILESYSTEM_STATS");
        break;

    case FilesystemCMD::RESET_CONFIGS:
        DEBUGLN("RESET_CONFIGS");
        break;

    case FilesystemCMD::SUCCESS:
        DEBUGLN("SUCCESS");
        break;


    default:
        DEBUGLN("Did not finish routing packet");
        return FilesystemCMD(-1);
    }

    return command;

}

bool FilesystemTask::downlink_directories(FileSystem* filesystem, const char* path)
{
    vector<String> filenames{};
    vector<uint8_t> data{};

    filenames = filesystem->listDir(path);
    
    for (auto filename : filenames) {
        data.push_back(filename.length());
        data.insert(data.end(), filename.begin(), filename.end());
    }
    
    BasePacket packet;
    packet.configure( (uint8_t)Topic::FILESYSTEM, (uint8_t)FilesystemCMD::LIST_DIR, data);
    packet.packetize();

    SerialComms::emit_packet(packet, interface);
    

    return true;
}


}