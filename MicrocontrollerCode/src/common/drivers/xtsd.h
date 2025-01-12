#pragma once

#include <SD.h>
#include "../globals.h"

namespace Cesium {
    
class XTSD {

private:
    uint64_t last_log_us;
    File dataFile;
    const char* filename = "/TestCode/test_dump.txt";

    uint8_t cs_pin;
    SPIClass* _spi;

    uint16_t last_log_duration_ms;
    bool log_success;

public:
    
    XTSD(uint8_t cs_pin, SPIClass* spi_instance);

    bool setup();
    bool log(const char* message);

    void open_filename(const char* filename);


    

};

}