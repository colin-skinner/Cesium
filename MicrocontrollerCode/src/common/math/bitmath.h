#pragma once
#include "../globals.h"
#include <Arduino.h>

namespace Cesium {

bool check_within_bits(int value, int bits, String title) {
    // DEBUGLN(value);
    // DEBUGLN(1 << bits);
    // Serial.println("AAAAAAAAAA");
    // TEST_MESSAGE("what");
    if(value >= (1 << bits)) {
        DEBUGLN(title + " not within " + bits + " bits (less than " + (1 << (bits-1)) + ")");
        return false;
    }
    
    if (value < 0) {
        DEBUGLN(title + " less than 0");
        return false;
    }

    return true;
}



}