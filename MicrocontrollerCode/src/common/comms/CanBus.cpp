#include "CanBus.h"
#include "../globals.h"
namespace Cesium {

CanBus::CanBus(uint8_t rx, uint8_t tx, uint16_t timeout)
: _timeout(timeout)
{
    this->setPins(rx, tx);
}

bool CanBus::setup(long baud_rate)
{
    uint8_t retries = 0;
    while (!this->begin(baud_rate)) {
        Serial.println("Starting CAN failed!");
        retries++;

        if (retries <= 30)
            return false;
    }

    return true;


}

bool CanBus::transmit(int id, const uint8_t *buffer, size_t len)
{
    size_t bytes_sent = 0;
    // Serial.println(id);
    // Repeat until data is sent
    while(bytes_sent < len) {
        
        // If fewer than 8 bytes remaining, write to end of buffer 
        size_t remaining = len - bytes_sent;

        if (remaining <= 8) {
            // Serial.println("hm");
            this->beginPacket(id);
            this->write(buffer + bytes_sent, len - bytes_sent);
            // Serial.println("yes");
            return this->endPacket();

        }
        

        // Otherwise write 8 bytes
        this->beginPacket(id);
        this->write(buffer + bytes_sent, 8);
        // Serial.println("oh");
        this->endPacket();

        // Serial.println("ah");
        
        remaining += 8;
        bytes_sent += 8;
    }
    return true;
}

// Wrapper for using it as a buffer
bool CanBus::transmit(int id, const std::vector<uint8_t> &vec, size_t len)
{
    const uint8_t* start = &vec[0];

    // If not input if given, or it is larger than vec.size()
    if (len == 0 || len > vec.size()) {
        len = vec.size();
    }

    return this->transmit(id, start, len);
}

size_t CanBus::receive(int &id, uint8_t *buffer)
{
    // Return 0 if packet size is 0
    size_t packet_size = this->parsePacket();
    if (packet_size == 0) {
        DEBUGLN("No CAN packet");
        return 0;
    }

    // Filters RTR (remote transmission requests)
    if (this->packetRtr()) {
        DEBUGLN("RTR");
        return 0;
    }

    // Packet ID
    id = this->packetId();
    DEBUG("packet with id 0x");
    DEBUG(id, HEX);


    // Stores 
    int index = 0;
    while (this->available()) {
        buffer[index] = this->read();
        index++;
    }

    // Asserts that number of bytes received is the same as the number actually read
    if (index != packet_size) {
        DEBUGLN("Byte number mismatch: bytes read - packet_size = " + String(index - (int)packet_size));
        return 0;
    }
    
    return packet_size;
}

size_t CanBus::receive(int &id, std::vector<uint8_t> &vec)
{
    vec.reserve(8);
    size_t len = receive(id, &vec[0]);
    vec.resize(len);
    return len;
}

}