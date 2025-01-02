#include "network.h"
#include "../globals.h"
namespace Cesium {

WirelessNetwork::WirelessNetwork() {}

bool WirelessNetwork::begin(const char* ssid, const char* password, bool connect) {
    
    _ssid = ssid;
    _password = password;

    if (connect) {
        wifi.begin(_ssid, _password);

        uint16_t timeout = 10'000; //milliseconds
        uint64_t start = millis();
        while (wifi.status() != WL_CONNECTED && 
               ( (millis() - start) < timeout)) {
            delay(500);
            DEBUG(".");
        }
        DEBUGLN("");

        // Timeout
        if (wifi.status() != WL_CONNECTED) {
            DEBUG("Timed out after ");
            DEBUG(float(timeout)/1000);
            DEBUG(" ms");
            return false;
        }

        DEBUG("Connected!");
    }

    return true;

}

bool WirelessNetwork::end()
{
    bool success = wifi.disconnect(true);
    success &= wifi.mode(WIFI_OFF);

    return success;
}

WirelessNetwork Network;

}

