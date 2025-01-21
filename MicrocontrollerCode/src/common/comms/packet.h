#pragma once

#include <Arduino.h>
#include <vector>
#include "../globals.h"
#include "cobs.h"

// TODO: Add error enum return codes



namespace Cesium {

enum packet_codes_t {
    BASE_PACKET_NO_ERR,
    BAD_TOPIC_LENGTH,
    BAD_COMMAND_LENGTH,
    BAD_DATA_LENGTH,
    BAD_MILLISTAMP_LENGTH,
    BAD_HEADER_LENGTH,
    BAD_PACKET_LENGTH,

    CRC_MISMATCH,
    DATA_LENGTH_MISMATCH,

    MISSING_MILLISTAMP,
    MISSING_HEADER,

    MILLISTAMP_OVERFLOW,

    BAD_COBS

};

class BasePacket {


private:
    size_t topic;
    size_t command;
    std::vector<uint8_t> data;
    uint32_t millistamp;
    size_t data_length;
    uint16_t crc;

    

    std::vector<uint8_t> header_bytes;
    std::vector<uint8_t> packet_bytes;
    // uint8_t packet_buffer[2056];
    

    static void extract_header_and_data(std::vector<uint8_t> &packet, std::vector<uint8_t> &header, std::vector<uint8_t> &data);

    // void cobs_encode();


public:
    static constexpr size_t TOPIC_BITS = 6;
    static constexpr size_t COMMAND_BITS = 4;
    static constexpr size_t DATA_LENGTH_BITS = 11;
    static constexpr size_t MILLISTAMP_BITS = 27;
    
    static constexpr size_t HEADER_LENGTH_BYTES = 6;
    static constexpr size_t CRC_BYTES = 2;
    // Total packet length = 2048 (2^11) + 6 + 2 = 2058

    static constexpr size_t MAX_TOPIC_ID = (1 << TOPIC_BITS)  - 1;
    static constexpr size_t MAX_COMMAND_ID = (1 << COMMAND_BITS)  - 1;
    static constexpr size_t MAX_DATA_LENGTH = (1 << DATA_LENGTH_BITS) - 1;
    static constexpr size_t MAX_MILLISTAMP_LENGTH = (1 << MILLISTAMP_BITS) - 1;

    BasePacket();

    packet_codes_t configure(size_t topic, size_t command, std::vector<uint8_t> &data);

    packet_codes_t stamp(bool use_sketch_time = false);

    packet_codes_t encode_header(bool stamp = true);
    static packet_codes_t decode_header(std::vector<uint8_t> &header_bytes, uint32_t &millistamp, size_t &topic, size_t &command, size_t &data_length);

    static uint32_t calc_crc16(std::vector<uint8_t> &data);

    packet_codes_t packetize(bool encode_header = true, bool stamp = true, bool cobs_encode = true);
    static packet_codes_t depacketize(const std::vector<uint8_t> &raw_data, BasePacket& packet, bool cobs_decode = true);

    // Getters for testing
    inline size_t get_topic() {return topic;} 
    inline size_t get_command() {return command;} 
    inline size_t get_data_length() {return data_length;} 
    inline size_t get_millistamp() {return millistamp;} 
    inline size_t get_crc() {return crc;} 

    inline const std::vector<uint8_t>& get_data() {return data;} 
    inline const std::vector<uint8_t>& get_header() {return header_bytes;} 
    inline const  std::vector<uint8_t>& get_packet() {return packet_bytes;} 

    inline void set_millistamp(uint32_t stamp) {millistamp = stamp;} 




    
};


} // namespace Cesium