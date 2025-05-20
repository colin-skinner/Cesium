#pragma once
#include <Arduino.h>
#include "../globals.h"

namespace Cesium {

#define MAX_LOGBUFFER_SIZE 128
class LogBuffer {

private:
    static uint8_t buff[MAX_LOGBUFFER_SIZE + 2]; // 1 for the delimeter

public:
    static void start();
    static bool write(uint8_t* buf_to_write, size_t length, size_t index);
    static uint8_t* get_buffer(size_t& size);

};
}