#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Driver code for SPX1276 radio

#include <Arduino.h>
#include <LoRa.h>
// #include "./comms/Comms.h"
// Including some comms packet thing

namespace Cesium {

class SPX1276 {
private:
    LoRaClass device;
    uint64_t last_transmit_us = 0;
    uint32_t transmit_time_ms = 0;
    String sample_packet = (String)"9980.00,23.71" + "," + (String)last_transmit_us + ",";

    // uint8_t _cs_pin;
    // uint8_t _rst_pin;
    // uint8_t _irq_pin;

    // uint8_t _rx_en;
    // uint8_t _tx_en;

    

public:

    SPX1276(uint8_t cs_pin, uint8_t rst_pin, uint8_t irq_pin, uint8_t tx_en, uint8_t rx_en, SPIClass* spi_instance);
    // void setBoostPins();
    bool setup();
    bool transmit(String packet, bool yield = true);
    bool receive();

    uint16_t syncWord = 0x45;
    uint64_t transmissionFreq = 915E6;
    const uint8_t RADIO_ADDRESS = 0x44;

    String packet;
};

} // namespace Cesium
