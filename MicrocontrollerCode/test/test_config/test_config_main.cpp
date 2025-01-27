#include <unity.h>
#include <Arduino.h>

#include "../test/test_config/test_config_main.h"

using namespace std;

void setup(){

    delay(2000);
    
    UNITY_BEGIN();
    test_all_load_json();
    UNITY_END();
}
void loop(){}