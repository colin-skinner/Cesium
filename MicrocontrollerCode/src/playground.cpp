#include <SPI.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>
#include "common/math/quaternion.h"
#define HSCK 14
#define HMISO 36
#define HMOSI 13
#define CS 33
SPIClass hspi(HSPI);
// Create GPS object
SFE_UBLOX_GNSS myGNSS;

void setup() {
    Serial.begin(115200);

    Quaternion<float> quat = {{0.3368608, -0.1684304, 0, -0.9263671}};

    Matrix3<float> mat = {{{-0.7163121,  0.6241135,  0.3120568},
                           {-0.6241135, -0.7730497,  0.1134752},
                           {0.3120568, -0.1134752,  0.9432624}}};

    auto result = quat_from_R(mat);

    printMatrix(result);

    while(1) {}
    while (!Serial); // Wait for Serial monitor
    hspi.begin(HSCK, HMISO, HMOSI);
    // Begin SPI communication with GPS
    if (!myGNSS.begin(hspi, CS, 10000000)) {
        Serial.println("Failed to initialize GPS over SPI.");
        while (1); // Halt if GPS fails
    }
    Serial.println("GPS initialized successfully over SPI!");
    // Set the GPS to output only UBX (faster than NMEA)
    myGNSS.setI2COutput(COM_TYPE_UBX);
    // Increase navigation frequency to 10 Hz (100 ms updates)
    myGNSS.setNavigationFrequency(10);
    // Optional: Configure dynamic model (e.g., for automotive use)
    // myGNSS.setDynamicModel(DYN_MODEL_AUTOMOTIVE);
    // Optional: Enable high precision mode
    // myGNSS.setHighPrecisionMode(true);
}
void loop() {
    // Check if new position data is available
    auto prev_time = micros();
    if (myGNSS.getPVT()) {
        Serial.print("Fix: ");
        Serial.print(myGNSS.getFixType());
        Serial.print(" | Lat: ");
        Serial.print(myGNSS.getLatitude() / 1e7, 6);
        Serial.print(" | Lon: ");
        Serial.print(myGNSS.getLongitude() / 1e7, 6);
        Serial.print(" | SIV: ");
        Serial.print(myGNSS.getSIV());
        Serial.print(" | Alt: ");
        Serial.print(myGNSS.getAltitudeMSL() / 1000.0, 2);
        Serial.println(" m");
    }
    Serial.println(micros() - prev_time);
    delay(100); // Poll every 100ms for new data (10Hz)
}