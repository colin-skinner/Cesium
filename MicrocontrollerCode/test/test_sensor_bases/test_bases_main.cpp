#include <unity.h>
#include <Arduino.h>

#include "../test/test_sensor_bases/test_bases_main.h"

using namespace std;

void setup(){

    delay(2000);
    Serial.begin(115200);
    // Serial.println("IN SETUP");
    UNITY_BEGIN();
    // Serial.println("AFTER BEGIN");
    
    test_all_sensor_base();
    test_all_accel_base();
    test_all_gyro_base();
    test_all_mag_base();
    UNITY_END();
}
void loop(){}