#include <unity.h>
#include <Arduino.h>

#include "common/math/vector.h"
using namespace std;



////////////////////////////////////////////////////////////
//                  Test zero_vector                      //
////////////////////////////////////////////////////////////

void test_zero_vector() {
    const int ROWS = 4;
    Vector<int, ROWS> zero_matrix = {{
        {0},
        {0},
        {0},
        {0}
    }};

    Vector<int, ROWS> test_matrix{};

    for (int row = 0; row < ROWS; row++) {
        TEST_ASSERT_EQUAL_INT(zero_matrix[row][0], test_matrix[row][0]);
    }
}

////////////////////////////////////////////////////////////
//                  Test equality                         //
////////////////////////////////////////////////////////////

void test_vector_float_equality() {
    const int ROWS = 3;

    Vector<float, ROWS> vec1 = {{
        {1.0},
        {3.0},
        {2.0}
    }};

    Vector<float, ROWS> vec2 = {{
        {1.0},
        {3.0},
        {2.0}
    }};


    TEST_ASSERT_TRUE(matrix_float_equals(vec1, vec2));
}

void test_vector_float_equality_edge_cases() {

    const int ROWS = 3;

    Vector<float, ROWS> vec1 = {{
        {1.0},
        {3.0},
        {2.0}
    }};

    Vector<float, ROWS> vec2 = {{
        {1.0},
        {3.0},
        {2.1}
    }};

    TEST_ASSERT_FALSE(matrix_float_equals(vec1, vec2));

    vec2 = {{
        {1.0},
        {3.0},
        {2.1}
    }};

    vec2[0][0] += 1e-7;

    TEST_ASSERT_FALSE(matrix_float_equals(vec1, vec2));
}

void test_vector_int_equality() {
    const int ROWS = 3;
    Vector<int, ROWS> vec1 = {{
        {1},
        {2},
        {5}
    }};

    Vector<int, ROWS> vec2 = {{
        {1},
        {2},
        {5}
    }};


    TEST_ASSERT_TRUE(matrix_int_equals(vec1, vec2));
}


void test_vector_int_equality_edge_cases() {
    const int ROWS = 2;
    const int COLS = 2;
    Matrix<int, ROWS, COLS> mat1 = {{
        {1,2},
        {3,4}
    }};

    Matrix<int, ROWS, COLS> mat2 = {{
        {2,2},
        {3,4}
    }};


    TEST_ASSERT_FALSE(matrix_int_equals(mat1, mat2));
}

////////////////////////////////////////////////////////////
//                    Dot Product                         //
////////////////////////////////////////////////////////////

void test_vector_dot_int() {
    const int ROWS = 7;

    Vector<int, ROWS> vec1 = {{
        {2},
        {3},
        {1},
        {3},
        {1},
        {5},
        {9},
    }};

    Vector<int, ROWS> result1 = {{
        {4},
        {9},
        {1},
        {9},
        {1},
        {25},
        {81},
    }};


    auto dot_result = dot(vec1, vec1);



    for (int i = 0; i < ROWS; i++)
        TEST_ASSERT_EQUAL_INT(result1[i][0], dot_result[i][0]);
}

void test_vector_dot_int_edge_cases() {
    const int ROWS = 7;

    Vector<int, ROWS> vec1 = {{
        {2},
        {4},
        {1},
        {3},
        {1},
        {5},
        {9},
    }};

    Vector<int, ROWS> result1 = {{
        {4},
        {6},
        {2},
        {6},
        {2},
        {10},
        {18},
    }};


    auto dot_result = dot(vec1, vec1);



    for (int i = 0; i < ROWS; i++)
        TEST_ASSERT_FALSE(matrix_int_equals(result1, dot_result));
}

void test_vector_dot_float() {
    const int ROWS = 7;

    Vector<float, ROWS> vec1 = {{
        {2},
        {3},
        {1},
        {3},
        {1},
        {5},
        {9},
    }};

    Vector<float, ROWS> result1 = {{
        {4},
        {9},
        {1},
        {9},
        {1},
        {25},
        {81},
    }};


    auto dot_result = dot(vec1, vec1);



    for (int i = 0; i < ROWS; i++)
        TEST_ASSERT_EQUAL_INT(result1[i][0], dot_result[i][0]);
}

void test_vector_dot_float_edge_cases() {
    const int ROWS = 7;

    Vector<float, ROWS> vec1 = {{
        {2},
        {4},
        {1},
        {3},
        {1},
        {5},
        {9},
    }};

    Vector<float, ROWS> result1 = {{
        {4},
        {6},
        {2},
        {6},
        {2},
        {10},
        {18},
    }};


    auto dot_result = dot(vec1, vec1);



    for (int i = 0; i < ROWS; i++)
        TEST_ASSERT_FALSE(matrix_float_equals(result1, dot_result));
}

////////////////////////////////////////////////////////////
//                 Test Cross Product                     //
////////////////////////////////////////////////////////////

void test_cross_int() {
    Vector3<int> vec1 = {{
        {1}, {2}, {3}
    }};

    Vector3<int> vec2 = {{
        {12}, {5}, {7}
    }};

    auto result = cross(vec1, vec2);

    Vector3<int> expected = {{
        {-1}, {29}, {-19}
    }};

    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_INT(expected[i][0], result[i][0]);
}

void test_cross_float() {
    Vector3<float> vec1 = {{
        {1}, {2}, {3}
    }};

    Vector3<float> vec2 = {{
        {12}, {5}, {7}
    }};

    auto result = cross(vec1, vec2);

    Vector3<float> expected = {{
        {-1}, {29}, {-19}
    }};

    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
}

void test_cross_zero_int() {
    Vector3<int> vec1 = {{
        {0}, {0}, {0}
    }};

    Vector3<int> vec2 = {{
        {12}, {5}, {7}
    }};

    auto result = cross(vec1, vec2);

    Vector3<int> expected = {{
        {0}, {0}, {0}
    }};

    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_INT(expected[i][0], result[i][0]);
}

void test_cross_zero_float() {
    Vector3<float> vec1 = {{
        {0}, {0}, {0}
    }};

    Vector3<float> vec2 = {{
        {12}, {5}, {7}
    }};

    auto result = cross(vec1, vec2);

    Vector3<float> expected = {{
        {0}, {0}, {0}
    }};

    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
}

////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_vector() {

    RUN_TEST(test_zero_vector);

    RUN_TEST(test_vector_float_equality);
    RUN_TEST(test_vector_float_equality_edge_cases);

    RUN_TEST(test_vector_int_equality);
    RUN_TEST(test_vector_int_equality_edge_cases);

    RUN_TEST(test_vector_dot_int);
    RUN_TEST(test_vector_dot_int_edge_cases);

    RUN_TEST(test_vector_dot_float);
    RUN_TEST(test_vector_dot_float_edge_cases);

    RUN_TEST(test_cross_int);
    RUN_TEST(test_cross_float);
    RUN_TEST(test_cross_zero_int);
    RUN_TEST(test_cross_zero_float);
}
