#include <unity.h>
#include <Arduino.h>
#include "common/comms/packet.h"
#include "common/comms/packet.cpp"
#include <vector>

using namespace std;
using namespace Cesium;


////////////////////////////////////////////////////////////
//                      Constructor                       //
////////////////////////////////////////////////////////////

void test_constructor() {
    BasePacket packet;
    TEST_ASSERT_EQUAL(packet.get_topic(), 0);
    TEST_ASSERT_EQUAL(packet.get_command(), 0);
    TEST_ASSERT_EQUAL(packet.get_data_length(), 0);
    TEST_ASSERT_EQUAL(packet.get_data().size(), 0);
    TEST_ASSERT_EQUAL(packet.get_header().size(), 0);
    TEST_ASSERT_EQUAL(packet.get_packet().size(), 0);
}

////////////////////////////////////////////////////////////
//                    Configuration                       //
////////////////////////////////////////////////////////////
// Topic, command edge cases

void test_configuration_base() {
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = 1;
    vector<uint8_t> data{};

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(packet.get_topic(), topic);
    TEST_ASSERT_EQUAL(packet.get_command(), command);
    TEST_ASSERT_EQUAL(packet.get_data_length(), 0);
}

////////////////////////////
//       Topic ID         //
////////////////////////////

void test_max_topic_id() {
    
    BasePacket packet;
    uint8_t topic = BasePacket::MAX_TOPIC_ID;
    uint8_t command = 1;
    vector<uint8_t> data{};


    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(packet.get_topic(), topic);
    TEST_ASSERT_EQUAL(packet.get_command(), command);
    TEST_ASSERT_EQUAL(packet.get_data_length(), 0);
}

void test_topic_id_edge_cases() {
    // Assert that configure return false for MAX_TOPIC_ID+1
    BasePacket packet;
    size_t topic = BasePacket::MAX_TOPIC_ID + 1;
    uint8_t command = 1;
    vector<uint8_t> data{};

    TEST_ASSERT_EQUAL(BAD_TOPIC_LENGTH, packet.configure(topic, command, data));
}

////////////////////////////
//      Command ID        //
////////////////////////////

void test_max_command_id() {
    
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = BasePacket::MAX_COMMAND_ID;
    vector<uint8_t> data{};


    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(packet.get_topic(), topic);
    TEST_ASSERT_EQUAL(packet.get_command(), command);
    TEST_ASSERT_EQUAL(packet.get_data_length(), 0);
}

void test_command_id_edge_cases() {
    BasePacket packet;
    size_t topic = 1;
    uint8_t command = BasePacket::MAX_COMMAND_ID + 1;;
    vector<uint8_t> data{};

    TEST_ASSERT_EQUAL(BAD_COMMAND_LENGTH, packet.configure(topic, command, data));
}

////////////////////////////
//      Data Length       //
////////////////////////////

void test_max_data_length() {
    
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = 1;
    vector<uint8_t> data(BasePacket::MAX_DATA_LENGTH, 12);

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(packet.get_topic(), topic);
    TEST_ASSERT_EQUAL(packet.get_command(), command);
    TEST_ASSERT_EQUAL(packet.get_data_length(), BasePacket::MAX_DATA_LENGTH);
    TEST_ASSERT_EQUAL(packet.get_data()[0], 12);
    TEST_ASSERT_EQUAL(packet.get_data()[BasePacket::MAX_DATA_LENGTH-1], 12);
}

void test_data_length_edge_cases() {
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = BasePacket::MAX_COMMAND_ID;
    vector<uint8_t> data(BasePacket::MAX_DATA_LENGTH + 1, 12);

    TEST_ASSERT_EQUAL(BAD_DATA_LENGTH, packet.configure(topic, command, data));
}

////////////////////////////////////////////////////////////
//                      Millistamp                        //
////////////////////////////////////////////////////////////

void test_stamp_max_millistamp() {
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = 1;
    vector<uint8_t> data{};

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.stamp());

    uint32_t milliseconds_in_day = 1000 * 3600 * 24;
    TEST_ASSERT_LESS_THAN_UINT32(milliseconds_in_day, packet.get_millistamp());
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(0, packet.get_millistamp());
}

void test_stamp_sketch_millistamp() {
    BasePacket packet;
    uint8_t topic = 1;
    uint8_t command = 1;
    vector<uint8_t> data{};

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.configure(topic, command, data));

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.stamp(true));

    uint32_t milliseconds_in_day = 1000 * 3600 * 24;
    TEST_ASSERT_LESS_THAN_UINT32(milliseconds_in_day, packet.get_millistamp());
    TEST_ASSERT_GREATER_THAN_UINT32(0, packet.get_millistamp());
}

////////////////////////////////////////////////////////////
//                     Encode Header                      //
////////////////////////////////////////////////////////////

void test_encode_header_must_stamp() {
    BasePacket packet;
    vector<uint8_t> data{};
    
    packet.configure(1, 1, data);

    TEST_ASSERT_EQUAL(MISSING_MILLISTAMP, packet.encode_header(false));
}

void test_encode_header_set_stamp() {
    /*
    # ["000 0001 0", -> millistamp
    #  "010 0011 0", -> millistamp
    #  "100 0101 0", -> millistamp
    #  "110 00000", -> millistamp, topic
    #  "1 0001 000", -> topic, command, data_length
    #  "00000100"] 4 -> data_length 
    */
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};
    
    packet.configure(1, 1, data);
    packet.set_millistamp(0x123456); // Artificial timestamp
    packet.encode_header(false);

    vector<uint8_t> expected = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};


    TEST_ASSERT_TRUE(expected == packet.get_header());
}

void test_encode_header_random_stamp() {
    /*
    # ["XXXXXXXX", -> millistamp
    #  "XXXXXXXX", -> millistamp
    #  "XXXXXXXX", -> millistamp
    #  "XXX 00000", -> millistamp, topic
    #  "1 0001 000", -> topic, command, data_length
    #  "00000100"] 4 -> data_length 
    */
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};
    
    packet.configure(1, 1, data);
    packet.encode_header();

    vector<uint8_t> expected = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};


    vector<uint8_t> header_bytes = packet.get_header();


    TEST_ASSERT_EQUAL_UINT8(0, header_bytes[3] & 0b00011111);
    TEST_ASSERT_EQUAL(0x88, header_bytes[4]);
    TEST_ASSERT_EQUAL(0x04, header_bytes[5]);
}

////////////////////////////////////////////////////////////
//                     Decode Header                      //
////////////////////////////////////////////////////////////

void test_decode_header_invalid_length() {
    vector<uint8_t> data{1,2,3,4};

    uint32_t millistamp;
    size_t topic, command, data_length;
    
    TEST_ASSERT_EQUAL(BAD_HEADER_LENGTH, BasePacket::decode_header(data, millistamp, topic, command, data_length));

}

void test_decode_header_set_stamp() {
    BasePacket packet;
    uint8_t expected_topic = 2;
    uint8_t expected_command = 5;
    vector<uint8_t> data{1,2,3,4};
    
    packet.configure(expected_topic, expected_command, data);
    packet.set_millistamp(0x123456); // Artificial timestamp
    packet.encode_header(false);
    

    uint32_t millistamp;
    size_t topic, command, data_length;
    BasePacket::decode_header(packet.get_header(), millistamp, topic, command, data_length);

    TEST_ASSERT_EQUAL_UINT32(0x123456, millistamp);
    TEST_ASSERT_EQUAL_UINT32(expected_topic, topic);
    TEST_ASSERT_EQUAL_UINT32(expected_command, command);
    TEST_ASSERT_EQUAL_UINT32(data.size(), data_length);
}

void test_decode_header_random_stamp() {
    BasePacket packet;
    uint8_t expected_topic = 2;
    uint8_t expected_command = 5;
    vector<uint8_t> data{1,2,3,4};
    
    packet.configure(expected_topic, expected_command, data);
    packet.encode_header();
    

    uint32_t millistamp;
    size_t topic, command, data_length;
    BasePacket::decode_header(packet.get_header(), millistamp, topic, command, data_length);

    TEST_ASSERT_EQUAL_UINT32(packet.get_millistamp(), millistamp);
    TEST_ASSERT_EQUAL_UINT32(expected_topic, topic);
    TEST_ASSERT_EQUAL_UINT32(expected_command, command);
    TEST_ASSERT_EQUAL_UINT32(data.size(), data_length);
}

////////////////////////////////////////////////////////////
//                          CRC                           //
////////////////////////////////////////////////////////////

void test_crc() {
    // Verified using https://crccalc.com/

    vector<uint8_t> data = {1,2,3,4,5,6};
    auto result = BasePacket::calc_crc16(data);

    TEST_ASSERT_EQUAL_UINT32(0xD90C, result);

    String test_string = "This is a test";
    data = vector<uint8_t>(test_string.begin(), test_string.end());

    result = BasePacket::calc_crc16(data);

    TEST_ASSERT_EQUAL_UINT32(0x982B, result);
}

////////////////////////////////////////////////////////////
//                       Packetize                        //
////////////////////////////////////////////////////////////

void test_packetize_must_encode_header() {
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};
    
    packet.configure(1, 1, data);



    TEST_ASSERT_EQUAL(MISSING_HEADER, packet.packetize(false));
}

void test_packetize_with_crc() {

    // Initalizing packet contents
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};

    // What the CRC and header SHOULD be with this data
    uint16_t expected_crc = 0x9C54;
    vector<uint8_t> expected_header = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};

    // What the full packet SHOULD look like (header + data + CRC)
    vector<uint8_t> full_packet = expected_header;
    full_packet.insert(full_packet.end(), data.begin(), data.end());
    full_packet.push_back((expected_crc >> 8) & 0xFF);
    full_packet.push_back(expected_crc & 0xFF);
    
    packet.configure(1, 1, data);

    // Artificial timestamp
    uint32_t test_millistamp = 0x123456;
    packet.set_millistamp(test_millistamp);

    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, packet.packetize(true, false));

    TEST_ASSERT_EQUAL(test_millistamp, packet.get_millistamp());

    TEST_ASSERT_EQUAL(expected_crc, packet.get_crc());
    
    TEST_ASSERT_TRUE(packet.get_packet() == full_packet);

}

////////////////////////////////////////////////////////////
//                      Depacketize                       //
////////////////////////////////////////////////////////////

void test_depacketize_min_length() {
    BasePacket packet;
    vector<uint8_t> data = {1,2,3,4};
    TEST_ASSERT_EQUAL(BAD_PACKET_LENGTH, BasePacket::depacketize(data, packet));
}

void test_depacketize_bad_crc() {
    // Initalizing packet contents
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};

    // What the CRC and header SHOULD be with this data
    uint16_t expected_crc = 0x9C54;
    vector<uint8_t> expected_header = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};

    // What the full packet SHOULD look like (header + data + CRC)
    vector<uint8_t> full_packet = expected_header;
    full_packet.insert(full_packet.end(), data.begin(), data.end());
    full_packet.push_back((expected_crc >> 8) & 0xFF);
    full_packet.push_back(expected_crc & 0xFF);

    full_packet.back()++; // BAD CRC

    BasePacket result;
    TEST_ASSERT_EQUAL(CRC_MISMATCH, BasePacket::depacketize(full_packet, result));
}

void test_depacketize_bad_data_length() {
    // Initalizing packet contents
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4,5};

    // What the CRC and header SHOULD be with this data
    uint16_t expected_crc = 0x4690; // New CRC when data has 5 elements
    vector<uint8_t> expected_header = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};

    // What the full packet SHOULD look like (header + data + CRC)
    vector<uint8_t> full_packet = expected_header;
    full_packet.insert(full_packet.end(), data.begin(), data.end());
    full_packet.push_back((expected_crc >> 8) & 0xFF);
    full_packet.push_back(expected_crc & 0xFF);


    BasePacket result;
    TEST_ASSERT_EQUAL(DATA_LENGTH_MISMATCH, BasePacket::depacketize(full_packet, result));
}

void test_depacketize_millistamp_too_large() {
    // Initalizing packet contents
    BasePacket packet;
    vector<uint8_t> data{1,2,3,4};

    // What the CRC and header SHOULD be with this data
    uint16_t expected_crc = 0xA7E1; // New CRC when millistamp changes

    // Changed MSB of first byte to 0xF 
    vector<uint8_t> expected_header = {0xF2, 0x46, 0x8A, 0xC0, 0x88, 0x04};

    // What the full packet SHOULD look like (header + data + CRC)
    vector<uint8_t> full_packet = expected_header;
    full_packet.insert(full_packet.end(), data.begin(), data.end());
    full_packet.push_back((expected_crc >> 8) & 0xFF);
    full_packet.push_back(expected_crc & 0xFF);


    BasePacket result;
    TEST_ASSERT_EQUAL(MILLISTAMP_OVERFLOW, BasePacket::depacketize(full_packet, result));
}

void test_depacketize_full() {
    // Initalizing packet contents
    vector<uint8_t> data{1,2,3,4};

    // What the CRC and header SHOULD be with this data
    uint16_t expected_crc = 0x9C54;
    vector<uint8_t> expected_header = {0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04};

    uint32_t millistamp;
    size_t topic, command, data_length;
    BasePacket::decode_header(expected_header, millistamp, topic, command, data_length);

    // What the full packet SHOULD look like (header + data + CRC)
    vector<uint8_t> full_packet = expected_header;
    full_packet.insert(full_packet.end(), data.begin(), data.end());
    full_packet.push_back((expected_crc >> 8) & 0xFF);
    full_packet.push_back(expected_crc & 0xFF);


    BasePacket result;
    TEST_ASSERT_EQUAL(BASE_PACKET_NO_ERR, BasePacket::depacketize(full_packet, result));

    TEST_ASSERT_EQUAL(millistamp, result.get_millistamp());
    TEST_ASSERT_EQUAL(topic, result.get_topic());
    TEST_ASSERT_EQUAL(command, result.get_command());
    TEST_ASSERT_EQUAL(data_length, result.get_data_length());
    TEST_ASSERT_TRUE(result.get_data() == data);
    TEST_ASSERT_TRUE(result.get_header() == expected_header);
    TEST_ASSERT_TRUE(result.get_packet() == full_packet);

}

////////////////////////////////////////////////////////////
//                      RUN IT ALL                        //
////////////////////////////////////////////////////////////

void run_packet_tests() {
    
    // Constructor
    RUN_TEST(test_constructor);

    // Configuration
    RUN_TEST(test_configuration_base);
    RUN_TEST(test_max_topic_id);
    RUN_TEST(test_topic_id_edge_cases);
    RUN_TEST(test_max_command_id);
    RUN_TEST(test_command_id_edge_cases);
    RUN_TEST(test_max_data_length);
    RUN_TEST(test_data_length_edge_cases);

    // Stamp
    RUN_TEST(test_stamp_max_millistamp);
    RUN_TEST(test_stamp_sketch_millistamp);

    // Encode header
    RUN_TEST(test_encode_header_must_stamp);
    RUN_TEST(test_encode_header_set_stamp);
    RUN_TEST(test_encode_header_random_stamp);

    // Decode header
    RUN_TEST(test_decode_header_invalid_length);
    RUN_TEST(test_decode_header_set_stamp);
    RUN_TEST(test_decode_header_random_stamp);

    // CRC
    RUN_TEST(test_crc);

    // Packetize
    RUN_TEST(test_packetize_must_encode_header);
    RUN_TEST(test_packetize_with_crc);

    // Depacketize
    RUN_TEST(test_depacketize_min_length);
    RUN_TEST(test_depacketize_bad_crc);
    RUN_TEST(test_depacketize_bad_data_length);
    RUN_TEST(test_depacketize_millistamp_too_large);
    RUN_TEST(test_depacketize_full);
    
}

void setup(){

    delay(2000);
    
    UNITY_BEGIN();
    run_packet_tests();
    UNITY_END();
}
void loop(){

}