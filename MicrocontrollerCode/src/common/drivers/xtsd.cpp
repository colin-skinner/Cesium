#include "Xtsd.h"

namespace Cesium {


XTSD::XTSD(uint8_t cs_pin, SPIClass *spi_instance) 
    : cs_pin{32}
    , _spi{&SPI}
    , last_log_us{0}
    , last_log_duration_ms{0}
    , log_success{true}
{}

bool XTSD::setup()
{
    if(!SD.begin(cs_pin, *_spi)) {
        Serial.println("initialization failed!");
        return false;
    }
    
    return true;
}

bool XTSD::log(const char* message)
{

    if (millis() - last_log_us > 5000) {
        dataFile.close();
        dataFile = SD.open(filename, FILE_APPEND);
        last_log_us = millis();
        DEBUGLN("Saved");
    }

    dataFile = SD.open(filename, FILE_APPEND);
        
    uint16_t start = millis();

    // Write to file
    if (dataFile) {
        
        DEBUG("Writing to ");
        DEBUG(filename);
        DEBUGLN("...");
        
        dataFile.println(message);

        dataFile.close();
        
        DEBUGLN("done.");
        log_success = true;
        
    }
    else {
        DEBUG("Error writing to ");
        DEBUGLN(filename);

        log_success = false;
        last_log_duration_ms = millis() - start;
    }

    last_log_duration_ms = millis() - start;
    return log_success;

}

void XTSD::open_filename(const char *filename)
{
    filename = filename;

    dataFile = SD.open(filename, FILE_APPEND);
}


}