#include <unity.h>
#include <Arduino.h>

#include "common/drivers/sensor_bases/MagnetometerBase.h"
#include "common/drivers/sensor_bases/MockBases.h"
#include "common/math/vector.h"

using namespace std;
using namespace Cesium::Sensor;


int mag_global = 0;
void mag_interrupt_function() {
    mag_global = 42;
}

////////////////////////////////////////////////////////////
//                  Constructor                           //
////////////////////////////////////////////////////////////

void test_mag_base_constructor() {
    MockMagBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_EQUAL(1000, sensor.get_read_interval_ms());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__i2c_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__spi_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__serial_instance());
    TEST_ASSERT_EQUAL(Interfaces::NOT_SET, sensor.get_interface());
    TEST_ASSERT_EQUAL_FLOAT(NAN, sensor.get_temp_C());
    Quaternion<float> test_matrix{};
    TEST_ASSERT_TRUE(test_matrix == sensor.get_body_to_sensor());

    Vector3<float> zero_vector = {{{0}, {0}, {0}}};

    TEST_ASSERT_TRUE(zero_vector == sensor.get_B_uT());
    TEST_ASSERT_TRUE(zero_vector == sensor.get_B_body_uT());
}

////////////////////////////////////////////////////////////
//                  Test Read                        //
////////////////////////////////////////////////////////////

void test_mag_base_read() {
    MockMagBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_TRUE(sensor.read());
    TEST_ASSERT_INT_WITHIN(10, millis(), sensor.get_last_read_time_ms());

    TEST_ASSERT_EQUAL_FLOAT(NAN, sensor.get_temp_C());

    Vector3<float> one_iter = {{{1.0}, {2.0}, {3.0}}};

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-10, one_iter[i][0], sensor.get_B_uT()[i][0]);
        TEST_ASSERT_FLOAT_WITHIN(1e-10, one_iter[i][0], sensor.get_B_body_uT()[i][0]);
    }

    Vector3<float> two_iter = {{{2.0}, {4.0}, {6.0}}};

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-10, one_iter[i][0], sensor.get_B_uT()[i][0]);
        TEST_ASSERT_FLOAT_WITHIN(1e-10, one_iter[i][0], sensor.get_B_body_uT()[i][0]);
    }

}

////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_mag_base() {

    RUN_TEST(test_mag_base_constructor);
    RUN_TEST(test_mag_base_read);

    // RUN_TEST(test_interrupt); // The testing function doesn't work unless pin 25 is connected to 3v3

}
