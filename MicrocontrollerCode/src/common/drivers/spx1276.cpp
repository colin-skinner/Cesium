#include "spx1276.h"
#include "../globals.h"

namespace Cesium {

SPX1276::SPX1276(uint8_t cs_pin, uint8_t rst_pin, uint8_t irq_pin, uint8_t tx_en, uint8_t rx_en, SPIClass* spi_instance)
    // : _tx_en(0)
    // , _rx_en(0)
{
    device.setPins(cs_pin, rst_pin, irq_pin);
    device.setBoostPins(tx_en, rx_en);
    device.setSPI(spi_instance);
}


bool SPX1276::setup()
{
    bool setup = false;
    for (int i=0; i<300; i++) {
        if(device.begin(transmissionFreq)) {
            DEBUGLN("found!");
            setup = true;
            break;
        }   
        DEBUGLN(".");
    }    

    if (!setup) {
        DEBUGLN("Failed to start SPX1276");
        return false;
    }

    device.setSyncWord(syncWord);
    DEBUGLN("LoRa Initializing OK!");

    return true;
}

bool SPX1276::transmit(String packet, bool yield)
{
    uint32_t oldTime = millis();

    if (device.beginPacket() == 0) {
        return true;
    }

    device.print(packet);
    device.endPacket(yield);
    transmit_time_ms = millis() - oldTime;  
    DEBUG("Transmit in ");
    DEBUG(transmit_time_ms);
    DEBUGLN("ms");
    return true;
}

bool SPX1276::receive()
{
    int packet_size = device.parsePacket();

    if (packet_size == 0) {
        return true;
    }

    int recipient = device.read();          // recipient address
    uint8_t sender = device.read();            // sender address
    uint8_t incomingMsgId = device.read();     // incoming msg ID
    uint8_t incomingLength = device.read();    // incoming msg length

    packet = "";

    while (device.available()) {
        packet += (char)device.read();
    }

    if (incomingLength != packet.length()) {   // check length for error
        DEBUGLN("error: message length does not match length");
        return false;                             // skip rest of function
    }

    // if the recipient isn't this device or broadcast,
    if (recipient != RADIO_ADDRESS && recipient != 0xFF) {
        DEBUGLN("This message is not for me.");
        return true;                             // skip rest of function
    }

    // if message is for this device, or broadcast, print details:
    DEBUGLN("Received from: 0x" + String(sender, HEX));
    DEBUGLN("Sent to: 0x" + String(recipient, HEX));
    DEBUGLN("Message ID: " + String(incomingMsgId));
    DEBUGLN("Message length: " + String(incomingLength));
    DEBUGLN("Message: " + packet);
    DEBUGLN("RSSI: " + String(LoRa.packetRssi()));
    DEBUGLN("Snr: " + String(LoRa.packetSnr()));
    DEBUGLN();
    
    return true;
}

}