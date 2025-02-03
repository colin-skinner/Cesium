#include <unity.h>
#include <Arduino.h>

#include "common/os/filesystem.h"
#include <vector>

#include "common/comms/PacketBroker.h"
#include "common/telemetry_tasks/FilesystemTask.h"

Cesium::FileSystem filesystem;

using namespace Cesium;
using namespace std;

MockSerial mock_port;

////////////////////////////////////////////////////////////
//                         Setup                          //
////////////////////////////////////////////////////////////

// Testing topic 5 (filesystem) command routing
void test_05_packet_routed(pair<FilesystemCMD, vector<uint8_t> > command) {
    BasePacket packet;

    packet.configure((size_t) Topic::FILESYSTEM, (size_t) command.first, command.second);
    packet.packetize();

    TEST_ASSERT_EQUAL(Topic::FILESYSTEM, PacketBroker::route_packet(packet));
    TEST_ASSERT_EQUAL(command.first, FilesystemTask::route_packet(packet));
}

// Creating test folder for testing
void start_filesystem() {

    filesystem.begin(true);
    filesystem.rmdir("/test", true);
    filesystem.mkdir("/test");

    FileSystem new_filesystem;

    // If filesystem is not mounted, this should mount and return true
    TEST_ASSERT_TRUE(new_filesystem.begin(true));

    // If filessytem is mounted, return true
    TEST_ASSERT_TRUE(new_filesystem.begin());
}

////////////////////////////////////////////////////////////
//                    list directories                    //
////////////////////////////////////////////////////////////

void test_create_files() {
    String result;
    vector<String> directories{};
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/test1.txt", "test1"));
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/TEST2.txt", "test2"));
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/test3.txt", "test3"));
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/test4.txt", "test4"));

    TEST_ASSERT_TRUE(filesystem.mkdir("/test/dir1"));
    TEST_ASSERT_TRUE(filesystem.mkdir("/test/dir2"));
    TEST_ASSERT_TRUE(filesystem.mkdir("/test/dir3"));

    directories = filesystem.listDir("/test");

    TEST_ASSERT_EQUAL(7, directories.size());

    // Alphabetical order

    TEST_ASSERT_EQUAL_STRING("TEST2.txt|5", directories[0].c_str());

    TEST_ASSERT_EQUAL_STRING("dir1", directories[1].c_str());
    TEST_ASSERT_EQUAL_STRING("dir2", directories[2].c_str());
    TEST_ASSERT_EQUAL_STRING("dir3", directories[3].c_str());

    TEST_ASSERT_EQUAL_STRING("test1.txt|5", directories[4].c_str());
    
    TEST_ASSERT_EQUAL_STRING("test3.txt|5", directories[5].c_str());
    TEST_ASSERT_EQUAL_STRING("test4.txt|5", directories[6].c_str());

}

void test_list_directory() {
    BasePacket list_dir_packet;
    vector<uint8_t> data{};

    list_dir_packet.configure((size_t) Topic::FILESYSTEM, (size_t) FilesystemCMD::LIST_DIR, data);

    FilesystemTask::downlink_directories(&filesystem, "/test");

    data = mock_port.packet.get_data();
    
    int index = 0;

    const char* expected = "TEST2.txt|5";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "dir1";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "dir2";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "dir3";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "test1.txt|5";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "test3.txt|5";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);

    expected = "test4.txt|5";
    TEST_ASSERT_EQUAL_INT(strlen(expected), data[index]);
    index++;
    TEST_ASSERT_EQUAL_INT(0, memcmp(&data[index], expected, strlen(expected)));
    index += strlen(expected);






}


////////////////////////////////////////////////////////////
//                    Packet Routing                      //
////////////////////////////////////////////////////////////

void test_filesystem_routing() {

    vector< pair< FilesystemCMD, vector<uint8_t> > > commands = {
        {FilesystemCMD::LIST_DIR, {}},
        {FilesystemCMD::WRITE_FILE, {}}, 
        {FilesystemCMD::READ_FILE, {}},
        {FilesystemCMD::DELETE_FILE, {}},
        {FilesystemCMD::FILESYSTEM_STATS, {}},
        {FilesystemCMD::RESET_CONFIGS, {}},
        {FilesystemCMD::SUCCESS, {}}
    };

    for (auto command : commands) {
        test_05_packet_routed(command);
    }
}
void run_all_filesystem_tests() {

    // Makes sure filesystem is running
    RUN_TEST(start_filesystem);

    SerialComms::set_mock_port(&mock_port);
    FilesystemTask::interface = MOCK_UART;

    RUN_TEST(test_filesystem_routing);
    

    RUN_TEST(test_create_files);
    RUN_TEST(test_list_directory);
    


}