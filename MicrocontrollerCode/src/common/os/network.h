#pragma once

#include <Arduino.h>
#include <WiFi.h>

namespace Cesium {

class WirelessNetwork {

public:
    WirelessNetwork();

    bool begin(const char* ssid, const char* password, bool connect = true);
    bool end();

    WiFiClass wifi;
    const char* _ssid     = "giraffe";
    const char* _password = "12481248";
};

}