#include <unity.h>
#include <Arduino.h>

#include "common/math/bitmath.h"

using namespace std;
using namespace Cesium;



////////////////////////////////////////////////////////////
//                  Test Within Bits                      //
////////////////////////////////////////////////////////////

void test_within_bits() {
    
    // 1 bit
    TEST_ASSERT_TRUE(check_within_bits(0, 1, ""));
    TEST_ASSERT_TRUE(check_within_bits(1, 1, ""));
    TEST_ASSERT_FALSE(check_within_bits(-1, 1, ""));
    TEST_ASSERT_FALSE(check_within_bits(2, 1, ""));

    // uint8_t
    TEST_ASSERT_TRUE(check_within_bits(0, 8, ""));
    TEST_ASSERT_TRUE(check_within_bits(1, 8, ""));
    TEST_ASSERT_TRUE(check_within_bits(255, 8, ""));
    TEST_ASSERT_FALSE(check_within_bits(-1, 8, ""));
    TEST_ASSERT_FALSE(check_within_bits(256, 8, ""));
}
////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_bitmath() {

    RUN_TEST(test_within_bits);
}
