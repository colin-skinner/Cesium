#include <unity.h>
#include <Arduino.h>

void simple_test(void)
{
    TEST_ASSERT_EQUAL(33, 33);
}

void run_example_tests() {
    
    RUN_TEST(simple_test);
}

void setup(){

    delay(2000);
    
    UNITY_BEGIN();

    run_example_tests();

    UNITY_END();
}

void loop(){

}