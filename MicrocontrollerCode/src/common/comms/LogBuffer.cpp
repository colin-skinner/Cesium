#include "LogBuffer.h"

namespace Cesium {

void LogBuffer::start()
{
    uint32_t stamp = millis();

    memcpy(buff, &stamp, sizeof(stamp));
}

bool LogBuffer::write(uint8_t *buf_to_write, size_t length, size_t index)
{
    if (index + length > MAX_LOGBUFFER_SIZE) {
        return false;
    }

    memcpy(buff + index, buf_to_write, length);
    return true;
}

uint8_t *LogBuffer::get_buffer(size_t &size)
{
    uint32_t stamp = millis();
    memcpy(buff + MAX_LOGBUFFER_SIZE - 4, &stamp, sizeof(stamp));

    // COBS encode this 
    return nullptr;
}
}