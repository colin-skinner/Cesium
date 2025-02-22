#include <unity.h>
#include <Arduino.h>

#include "../test/test_tasks/test_packet_headers.h"
#include <vector>

using namespace std;


void setup(){

    delay(2000);
    Serial.begin(115200);
    UNITY_BEGIN();
    run_all_system_status_tests();
    run_all_clock_tests();
    run_all_filesystem_tests();
    UNITY_END();
}
void loop(){}