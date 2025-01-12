#include <unity.h>
#include <Arduino.h>
#include "common/clock.h"

using namespace std;
using namespace Cesium;


////////////////////////////////////////////////////////////
//                     Jump clock                         //
////////////////////////////////////////////////////////////

void test_set_time() {
    TEST_ASSERT_TRUE(Clock::jump_clock(1111, 1, 1, 2000));
}

void test_max_time_error() {
    TEST_ASSERT_FALSE(Clock::jump_clock(MILLISECS_PER_DAY + 1, 1, 1, 2000));
}

// Does ESP clock work correctly


////////////////////////////////////////////////////////////
//              Get millis since midnight                 //
////////////////////////////////////////////////////////////

void test_get_millis() {
    Clock::jump_clock(1111, 1, 1, 2000);
    uint32_t expected_millis = Clock::millis_since_midnight - Clock::millis_last_time_sync + millis();
    TEST_ASSERT_TRUE( (Clock::get_millis_since_midnight() - expected_millis) < 1000  ); // Hopefully this is within 1 second
    TEST_ASSERT_TRUE(Clock::get_millis_since_midnight() < MILLISECS_PER_DAY);
}

void test_multiple_day_millis() {
    Clock::millis_last_time_sync = MILLISECS_PER_DAY * 2 + 1000;
    TEST_ASSERT_TRUE( (Clock::get_millis_since_midnight() - (Clock::millis_last_time_sync % MILLISECS_PER_DAY)) < 1000  ); // Hopefully this is within 1 second
    TEST_ASSERT_TRUE(Clock::get_millis_since_midnight() < MILLISECS_PER_DAY);
}

////////////////////////////////////////////////////////////
//                      RUN IT ALL                        //
////////////////////////////////////////////////////////////

void run_tests() {
    
    // Jump clock
    RUN_TEST(test_set_time);
    RUN_TEST(test_max_time_error);

    // Get millis
    RUN_TEST(test_get_millis);
    RUN_TEST(test_multiple_day_millis);

    
}

void setup(){

    delay(2000);
    
    UNITY_BEGIN();
    run_tests();
    UNITY_END();
}
void loop(){

}