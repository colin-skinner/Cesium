#include <unity.h>
#include <Arduino.h>

#include "common/drivers/sensor_bases/GpsBase.h"
#include "common/drivers/sensor_bases/MockBases.h"
#include "common/math/vector.h"

using namespace std;
using namespace Cesium::Sensor;

////////////////////////////////////////////////////////////
//                  Constructor                           //
////////////////////////////////////////////////////////////

void test_gps_base_constructor() {
    MockGpsBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_EQUAL(1000, sensor.get_read_interval_ms());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__i2c_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__spi_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__serial_instance());
    TEST_ASSERT_EQUAL(Interfaces::NOT_SET, sensor.get_interface());
    TEST_ASSERT_EQUAL_FLOAT(NAN, sensor.get_temp_C());

    TEST_ASSERT_EQUAL_INT(0, sensor.get_latitude_scaled());
    TEST_ASSERT_EQUAL_INT(0, sensor.get_longitude_scaled());
    TEST_ASSERT_EQUAL_FLOAT(0, sensor.get_latitude_deg());
    TEST_ASSERT_EQUAL_FLOAT(0, sensor.get_longitude_deg());
    TEST_ASSERT_EQUAL_FLOAT(0, sensor.get_altitude_m());
    TEST_ASSERT_EQUAL_INT(0, sensor.get_satellites_in_view());
    TEST_ASSERT_EQUAL_FLOAT(0, sensor.get_heading_scaled());
    TEST_ASSERT_EQUAL_FLOAT(0, sensor.get_ground_speed_mmps());
}

////////////////////////////////////////////////////////////
//                  Test Read                        //
////////////////////////////////////////////////////////////

void test_gps_base_read() {
    MockGpsBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_TRUE(sensor.read());
    TEST_ASSERT_INT_WITHIN(10, millis(), sensor.get_last_read_time_ms());

    TEST_ASSERT_EQUAL_FLOAT(NAN, sensor.get_temp_C());

    TEST_ASSERT_EQUAL_INT(420, sensor.get_latitude_scaled());
    TEST_ASSERT_EQUAL_INT(42000, sensor.get_longitude_scaled());
    TEST_ASSERT_EQUAL_FLOAT(12.1, sensor.get_latitude_deg());
    TEST_ASSERT_EQUAL_FLOAT(12.2, sensor.get_longitude_deg());
    TEST_ASSERT_EQUAL_FLOAT(10000.4, sensor.get_altitude_m());
    TEST_ASSERT_EQUAL_INT(12, sensor.get_satellites_in_view());
    TEST_ASSERT_EQUAL_INT(1.1, sensor.get_heading_scaled());
    TEST_ASSERT_EQUAL_INT(18, sensor.get_ground_speed_mmps());

}

////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_gps_base() {

    RUN_TEST(test_gps_base_constructor);
    RUN_TEST(test_gps_base_read);
    // RUN_TEST(test_interrupt); // The testing function doesn't work unless pin 25 is connected to 3v3

}
