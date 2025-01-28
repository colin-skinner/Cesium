#include <unity.h>
#include <Arduino.h>

#include "common/drivers/sensor_bases/SensorBase.h"
#include "common/drivers/sensor_bases/MockBases.h"
#include "common/math/vector.h"
#include <Wire.h>
#include <SPI.h>

using namespace std;
using namespace Cesium::Sensor;


int global = 0;
void interrupt_function() {
    global = 42;
}

////////////////////////////////////////////////////////////
//                  Constructor                           //
////////////////////////////////////////////////////////////

void test_sensor_base_constructor() {
    MockSensorBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_EQUAL(1000, sensor.get_read_interval_ms());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__i2c_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__spi_instance());
    TEST_ASSERT_EQUAL(nullptr, sensor.get__serial_instance());
    TEST_ASSERT_EQUAL(Interfaces::NOT_SET, sensor.get_interface());
    TEST_ASSERT_EQUAL(NULL, sensor.get_temp_C());
    Matrix3<float> test_sensor_base_matrix{};
    TEST_ASSERT_TRUE(test_sensor_base_matrix == sensor.get_body_to_sensor());
}

////////////////////////////////////////////////////////////
//                  Test Interfaces                       //
////////////////////////////////////////////////////////////

void test_sensor_base_set_i2c() {
    MockSensorBase sensor;

    TEST_ASSERT_EQUAL(nullptr, sensor.get__i2c_instance());
    sensor.attach_I2C(&Wire);
    TEST_ASSERT_EQUAL(&Wire, sensor.get__i2c_instance());
    TEST_ASSERT_EQUAL(Interfaces::I2C, sensor.get_interface());
}

void test_sensor_base_set_spi() {
    MockSensorBase sensor;

    TEST_ASSERT_EQUAL(nullptr, sensor.get__spi_instance());
    sensor.attach_SPI(&SPI);
    TEST_ASSERT_EQUAL(&SPI, sensor.get__spi_instance());
    TEST_ASSERT_EQUAL(Interfaces::SPI, sensor.get_interface());
}

void test_sensor_base_set_serial() {
    MockSensorBase sensor;

    TEST_ASSERT_EQUAL(nullptr, sensor.get__serial_instance());
    sensor.attach_serial(&Serial);
    TEST_ASSERT_EQUAL(&Serial, sensor.get__serial_instance());
    TEST_ASSERT_EQUAL(Interfaces::Serial, sensor.get_interface());
}

////////////////////////////////////////////////////////////
//                  Test Read                        //
////////////////////////////////////////////////////////////

void test_sensor_base_read() {
    MockSensorBase sensor;

    TEST_ASSERT_EQUAL(0, sensor.get_last_read_time_ms());
    TEST_ASSERT_TRUE(sensor.read());
    TEST_ASSERT_INT_WITHIN(100, millis(), sensor.get_last_read_time_ms());
    TEST_ASSERT_EQUAL(NULL, sensor.get_temp_C());
}

////////////////////////////////////////////////////////////
//                  Test Interrupt                        //
////////////////////////////////////////////////////////////

void test_sensor_base_interrupt() {

    MockSensorBase sensor;
    Serial.println(1);
    TEST_ASSERT_EQUAL(0, global);
    Serial.println(2);
    sensor.attach_int_pin(25, interrupt_function, ONHIGH);
    Serial.println(3);
    pinMode(25, PULLUP);
    Serial.println(4);
    digitalWrite(25, HIGH);
    delay(100);
    Serial.println(5);
    TEST_ASSERT_EQUAL(42, global);


}
////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_sensor_base() {

    RUN_TEST(test_sensor_base_constructor);

    RUN_TEST(test_sensor_base_set_i2c);
    RUN_TEST(test_sensor_base_set_spi);
    RUN_TEST(test_sensor_base_set_serial);
    RUN_TEST(test_sensor_base_read);

    // RUN_TEST(test_sensor_base_interrupt); // The testing function doesn't work unless pin 25 is connected to 3v3

}


