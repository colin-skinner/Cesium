#include "packet.h"
#include "../math/bitmath.h"
#include "crc16.h"
#include "../clock.h"
#include "cobs.h"

using namespace std;
namespace Cesium {

BasePacket::BasePacket() : topic{0}, command{0}, millistamp{0}, crc{0}, data{}, data_length{0}, header_bytes{}, packet_bytes{}
{
}

packet_codes_t BasePacket::configure(size_t topic, size_t command, vector<uint8_t> &data)
{
    

    RETURN_WITH_CODE_IF_FALSE(BAD_TOPIC_LENGTH, check_within_bits(topic, TOPIC_BITS, "topic"));
    RETURN_WITH_CODE_IF_FALSE(BAD_COMMAND_LENGTH, check_within_bits(command, COMMAND_BITS, "command"));

    this->data_length = data.size();
    RETURN_WITH_CODE_IF_FALSE(BAD_DATA_LENGTH, check_within_bits(data_length, DATA_LENGTH_BITS, "data_length"));

    this->topic = topic;
    this->command = command;
    this->data = data;
    
    return BASE_PACKET_NO_ERR;
}

packet_codes_t BasePacket::stamp(bool use_sketch_time)
{
    // NOT IMPLEMENTED YET, IMPLEMENT TIME SYNC FIRST

    if (use_sketch_time) {
        this->millistamp = millis();
    }
    else {
        this->millistamp = Clock::get_millis_since_midnight();
    }

    RETURN_WITH_CODE_IF_FALSE(BAD_MILLISTAMP_LENGTH, check_within_bits(millistamp, MILLISTAMP_BITS, "millistamp"));

    return BASE_PACKET_NO_ERR;
}

packet_codes_t BasePacket::encode_header(bool stamp)
{
    if (stamp)
        this->stamp();

    if (this->millistamp == 0) {
        DEBUG("Must add millistamp before encoding");
        return MISSING_MILLISTAMP;
    }

    this->header_bytes.clear();
    
    uint8_t b0 = (millistamp >> 19) & 0xFF; // Millistamp (8)
    uint8_t b1 = (millistamp >> 11) & 0xFF; // Millistamp (8)
    uint8_t b2 = (millistamp >> 3) & 0xFF; // Millistamp (8)

    uint8_t b3 = ((millistamp << 5) & 0b11100000) + (( (topic >> 1) & 0b00011111)) & 0xFF; // Millistamp (3) and topic (5)
    uint8_t b4 = ((( (topic << 7) & 0b10000000) + ( (command << 3) & 0b01111000) + ( (data_length >> 8) & 0b00000111))) & 0xFF; // Topic (1), command (4), and data_length (3)
    uint8_t b5 = (data_length) & 0xFF; // data_length(8)


    header_bytes = {b0, b1, b2, b3, b4, b5};


    return BASE_PACKET_NO_ERR;
}

packet_codes_t BasePacket::decode_header(vector<uint8_t> &header_bytes, uint32_t &millistamp, size_t &topic, size_t &command, size_t &data_length)
{

    if (header_bytes.size() != 6) {
        DEBUGLN("Header must have 6 bytes");
        return BAD_HEADER_LENGTH;
    }

    millistamp = (
        (int(header_bytes[0]) << 19)
        + (int(header_bytes[1]) << 11)
        + (int(header_bytes[2]) << 3)
        + (int(header_bytes[3] & 0b11100000) >> 5)
    );

    topic = (
        (int(header_bytes[3] & 0b00011111) << 1 )
        + (int(header_bytes[4] & 0b10000000) >> 7)
    );

    command = (
        (int(header_bytes[4] & 0b01111000) >> 3)
    );

    data_length = (
        (int(header_bytes[4] & 0b00000111) << 8)
        + int(header_bytes[5])
    );

    return BASE_PACKET_NO_ERR;
}

uint32_t BasePacket::calc_crc16(vector<uint8_t> &data)
{
    return crc16xmodem(data);
}

packet_codes_t BasePacket::packetize(bool encode_header, bool stamp, bool cobs_encode)
{
    if (encode_header) {
        this->encode_header(stamp);
    }

    if (header_bytes.size() == 0) {
        DEBUGLN("Must encode header before packetizing");
        return MISSING_HEADER;
    }

    // Full length of packet
    size_t full_packet_length = HEADER_LENGTH_BYTES + data_length + CRC_BYTES;

    // Checks packet length - may remove for performance reasons
    // if (header_bytes.size() + data.size() + CRC_BYTES != full_packet_length) {
    //     return BAD_PACKET_LENGTH;
    // }

    vector<uint8_t> raw_data;

    raw_data.clear();
    raw_data = header_bytes;

    raw_data.reserve(full_packet_length);
    raw_data.insert(raw_data.end(), data.begin(), data.end());

    if (raw_data.size() + 2 != full_packet_length) {
        return BAD_PACKET_LENGTH;
    }
    this->crc = calc_crc16(raw_data);

    raw_data.push_back((crc >> 8) & 0xFF);
    raw_data.push_back(crc & 0xFF);

    if (cobs_encode) {
        // COBS encode
        if (CobsTranscoder::Encode(raw_data, this->packet_bytes) == false) {
            return BAD_COBS;
        }
    }
    else {
        this->packet_bytes = raw_data;
    }
    

    return BASE_PACKET_NO_ERR;
}

void BasePacket::extract_header_and_data(vector<uint8_t> &packet, vector<uint8_t> &header, vector<uint8_t> &data)
{
    header = vector<uint8_t>(packet.begin(), packet.begin() + 6); 
    data = vector<uint8_t>(packet.begin() + 6, packet.end()); 
}

packet_codes_t BasePacket::depacketize(const std::vector<uint8_t> &raw_data, BasePacket &packet, bool cobs_decode)
{
    vector<uint8_t> received_bytes;
    
    if (cobs_decode) {
        // COBS decode
        if (CobsTranscoder::Decode(raw_data, received_bytes) == false) {
            return BAD_COBS;
        }

        received_bytes.pop_back(); // Delete 0 at end
    }
    else {
        received_bytes = raw_data;
    }

    packet.packet_bytes = received_bytes;

    // for (auto char_str : received_bytes) {
    //     DEBUG(char_str, HEX);
    //     DEBUG(" ");
    // }
    // DEBUGLN();

    if (received_bytes.size() < 8) {
        DEBUGLN("Packet must 8 bytes or larger");
        return BAD_PACKET_LENGTH;
    }

    
    // Implicit cast to 16 bits
    // Remove CRC from received bytes
    uint16_t crc_LSB = received_bytes.back(); 
    received_bytes.pop_back();
    uint16_t crc_MSB = received_bytes.back(); 
    received_bytes.pop_back();

    uint16_t received_crc = ((crc_MSB << 8) & 0xFF00) + (crc_LSB & 0x00FF);

    uint16_t decoded_crc = crc16xmodem(received_bytes);

    if (received_crc != decoded_crc) {
        DEBUGLN("Packet CRC 0x" + String(received_crc, HEX) + " does not match decoded CRC 0x" + String(decoded_crc, HEX));
        return CRC_MISMATCH;
    }

    packet.crc = received_crc;
    
    // Extract header from data, and decode header into BasePacket object
    extract_header_and_data(received_bytes, packet.header_bytes, packet.data);

    decode_header(packet.header_bytes, packet.millistamp, packet.topic, packet.command, packet.data_length);

    // Verify data length
    if (packet.data.size() != packet.data_length) {
        DEBUGLN("Packet data length " + String(packet.data_length) + " does not match actual data length" + String(packet.data.size()));
        return DATA_LENGTH_MISMATCH;
    }

    // Verify millistamp fits within a day
    if (packet.millistamp >= MILLISECS_PER_DAY) {
        DEBUGLN("Packet millistamp " + String(packet.millistamp) + " does not fit within the length of a day (86400000 ms)");
        return MILLISTAMP_OVERFLOW;
    }

    return BASE_PACKET_NO_ERR;
}

}