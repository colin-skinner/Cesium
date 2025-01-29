#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: 1.X.X
// PURPOSE: Wrapper code for CAN Bus buffers

#include <Arduino.h>
#include <CAN.h>
#include <vector>

namespace Cesium {

class CanBus : public ESP32SJA1000Class {
private:
    uint16_t timeout;
public:

    CanBus() : timeout{0} { setPins(26, 25); };
    CanBus(uint8_t rx, uint8_t tx);

    inline void set_timeout(uint16_t timeout_ms) {timeout = timeout_ms;}

    // Wrapper around transmission. True if transmit within 
    bool transmit(int id, const uint8_t* buffer, size_t len);

    // Transmits vector, if len unspecified, transmits all of vector
    bool transmit(int id, const std::vector<uint8_t>& vec, size_t len = 0);
    
    // Wrapper around receiving. returns num of bytes received. buffer must be >8 bytes
    size_t receive(int& id, uint8_t* buffer);
    size_t receive(int& id, std::vector<uint8_t>& vec);



};


///////// Use ////////////

// CanBus can(rx_pin, tx_pin);
// can.begin(500e3);
// can.transmit(12, buff, 24);

// https://github.com/sandeepmistry/arduino-CAN/blob/master/API.md

} // namespace Cesium