#include <unity.h>
#include <Arduino.h>

#include "../test/test_math/test_math_main.h"

using namespace std;

void setup(){

    delay(2000);
    
    UNITY_BEGIN();
    test_all_bitmath();
    test_all_matrix();
    test_all_vector();
    UNITY_END();
}
void loop(){}