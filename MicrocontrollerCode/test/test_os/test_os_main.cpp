#include <unity.h>
#include <Arduino.h>

#include "../test/test_os/test_os_main.h"
#include <vector>

using namespace std;

void setup(){

    delay(2000);
    
    UNITY_BEGIN();
    run_all_filesystem_tests();
    UNITY_END();
}
void loop(){}