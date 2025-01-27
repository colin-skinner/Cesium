#include <unity.h>
#include <Arduino.h>

#include "config/ConfigReader.h"
#include "jsons.h"

using namespace std;
using namespace Cesium::File;

////////////////////////////////////////////////////////////
//                 Test Loading JSON                      //
////////////////////////////////////////////////////////////

void test_loading_json() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(good_json, doc));
}

void test_loading_json_bad() {
    JsonDocument doc;
    TEST_ASSERT_FALSE(json_open(bad_json, doc));
}

////////////////////////////////////////////////////////////
//               Test extracting variables                //
////////////////////////////////////////////////////////////

void test_extracting_basic() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(good_json, doc));

    int test_int;
    float test_float;
    String test_string;

    TEST_ASSERT_TRUE(json_extract(doc, "int", test_int));
    TEST_ASSERT_EQUAL_INT(1, test_int);

    TEST_ASSERT_TRUE(json_extract(doc, "float", test_float));
    TEST_ASSERT_EQUAL_FLOAT(9.999, test_float);

    TEST_ASSERT_TRUE(json_extract(doc, "string", test_string));
    TEST_ASSERT_EQUAL_STRING("file", test_string.c_str());
}

void test_extracting_basic_bad_input() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(good_json, doc));

    int test_int;
    float test_float;
    String test_string;

    TEST_ASSERT_FALSE(json_extract(doc, "int_BAD", test_int));

    TEST_ASSERT_FALSE(json_extract(doc, "float_BAD", test_float));

    TEST_ASSERT_FALSE(json_extract(doc, "string_BAD", test_string));
}

void test_extracting_vector() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(vectors, doc));

    Vector3<int> test_int_vec;
    Vector3<float> test_float_vec;
    Vector3<int> expected = {{
        {1},
        {2},
        {3}
    }};
    Vector3<float> expected_float = {{
        {1.0},
        {2.0},
        {3.0}
    }};
    // Serial.println(1);
    TEST_ASSERT_TRUE((json_extract<int,3,1>(doc, "good_vector", test_int_vec)));
    // Serial.println(2);
    TEST_ASSERT_TRUE((json_extract<float,3,1>(doc, "good_vector", test_float_vec)));
    // Serial.println(3);
    TEST_ASSERT_TRUE(matrix_int_equals(expected, test_int_vec));
    // Serial.println(4);
    TEST_ASSERT_TRUE(matrix_float_equals(expected_float, test_float_vec));
}

void test_bad_rows_and_cols() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(vectors, doc));

    Vector3<int> test_int_vec;

    TEST_ASSERT_FALSE((json_extract<int,3,1>(doc, "row_mismatch", test_int_vec)));
    TEST_ASSERT_FALSE((json_extract<int,3,1>(doc, "bad_col_size", test_int_vec)));
}

void test_bad_vector_type() {
    JsonDocument doc;
    TEST_ASSERT_TRUE(json_open(vectors, doc));

    Vector3<float> test_int_vec;

    TEST_ASSERT_FALSE((json_extract<float,3,1>(doc, "bad_datatype", test_int_vec)));
}

////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_load_json() {

    RUN_TEST(test_loading_json);
    RUN_TEST(test_loading_json_bad);

    RUN_TEST(test_extracting_basic);
    RUN_TEST(test_extracting_basic_bad_input);
    RUN_TEST(test_extracting_vector);
    RUN_TEST(test_bad_rows_and_cols);
    RUN_TEST(test_bad_vector_type);
}
