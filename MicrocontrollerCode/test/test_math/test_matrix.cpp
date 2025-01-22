#include <unity.h>
#include <Arduino.h>

#include "common/math/matrix.h"
using namespace std;



////////////////////////////////////////////////////////////
//                  Test zero_matrix                      //
////////////////////////////////////////////////////////////

void test_zero_matrix() {
    const int ROWS = 4;
    const int COLS = 5;
    Matrix<int, ROWS, COLS> zero_matrix = {{
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    }};

    Matrix<int, ROWS, COLS> test_matrix{};

    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            TEST_ASSERT_EQUAL_INT(zero_matrix[row][col], test_matrix[row][col]);
        }
    }
}

////////////////////////////////////////////////////////////
//                  Test equality                         //
////////////////////////////////////////////////////////////

void test_matrix_float_equality() {
    const int ROWS = 2;
    const int COLS = 2;
    Matrix<float, ROWS, COLS> mat1 = {{
        {1.0,2.0},
        {3.0,4.0}
    }};

    Matrix<float, ROWS, COLS> mat2 = {{
        {1.0,2.0},
        {3.0,4.0}
    }};


    TEST_ASSERT_TRUE(matrix_float_equals(mat1, mat2));
}

void test_matrix_float_equality_edge_cases() {

    const int ROWS = 2;
    const int COLS = 2;
    Matrix<float, ROWS, COLS> mat1 = {{
        {1.0,2.0},
        {3.0,4.0}
    }};

    Matrix<float, ROWS, COLS> mat2 = {{
        {1.1,2.0},
        {3.0,4.0}
    }};

    TEST_ASSERT_FALSE(matrix_float_equals(mat1, mat2));

    mat2 = {{
        {1.0,2.0},
        {3.0,4.0}
    }};

    mat2[0][0] += 1e-7;

    TEST_ASSERT_FALSE(matrix_float_equals(mat1, mat2));
}

void test_matrix_int_equality() {
    const int ROWS = 2;
    const int COLS = 2;
    Matrix<int, ROWS, COLS> mat1 = {{
        {1,2},
        {3,4}
    }};

    Matrix<int, ROWS, COLS> mat2 = {{
        {1,2},
        {3,4}
    }};


    TEST_ASSERT_TRUE(matrix_int_equals(mat1, mat2));
}


void test_matrix_int_equality_edge_cases() {
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
//                    Multiplication                      //
////////////////////////////////////////////////////////////

void test_matrix_multiplication_int() {
    const int ROWS = 2;
    const int INT_DIM = 7;
    const int COLS = 2;
    Matrix<int, ROWS, INT_DIM> mat1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<int, INT_DIM, COLS> mat2 = {{
        {2,2},
        {3,4},
        {1,2},
        {3,4},
        {1,0},
        {5,4},
        {9,9},
    }};

    Matrix<int, ROWS, COLS> small_product = {{
        {121, 119},
        {169, 169}
    }};

    Matrix<int, INT_DIM, INT_DIM> large_product = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    auto small_result = mat1*mat2; // 2x7 * 7x2 = 2x2

    auto large_result = mat2*mat1; // 7x2 * 2x7 = 7x7

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            TEST_ASSERT_EQUAL(small_result[i][j], small_product[i][j]);

    for (int i = 0; i < INT_DIM; i++)
        for (int j = 0; j < INT_DIM; j++)
            TEST_ASSERT_EQUAL(large_result[i][j], large_product[i][j]);
}

void test_matrix_multiplication_float() {
    const int ROWS = 2;
    const int INT_DIM = 7;
    const int COLS = 2;
    Matrix<float, ROWS, INT_DIM> mat1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<float, INT_DIM, COLS> mat2 = {{
        {2,2},
        {3,4},
        {1,2},
        {3,4},
        {1,0},
        {5,4},
        {9,9},
    }};

    Matrix<float, ROWS, COLS> small_product = {{
        {121, 119},
        {169, 169}
    }};

    Matrix<float, INT_DIM, INT_DIM> large_product = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    auto small_result = mat1*mat2; // 2x7 * 7x2 = 2x2

    auto large_result = mat2*mat1; // 7x2 * 2x7 = 7x7

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            TEST_ASSERT_EQUAL_FLOAT(small_result[i][j],small_product[i][j]);

    for (int i = 0; i < INT_DIM; i++)
        for (int j = 0; j < INT_DIM; j++)
            TEST_ASSERT_EQUAL_FLOAT(large_result[i][j],large_product[i][j]);
}

////////////////////////////////////////////////////////////
//                    Test Addition                       //
////////////////////////////////////////////////////////////

void test_matrix_addition_int() {


    Matrix<int, 2, 7> mat1_1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<int, 2, 7> mat1_2 = {{
        {5,1,8,12,5,6,7},
        {3,4,5,6,6,6,9}
    }};

    Matrix<int, 2, 7> expected_result1 = {{
        {6,3,11,16,10,12,14},
        {6,8,10,12,13,14,18}
    }};

    auto result = mat1_1 + mat1_2;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL(result[i][j], expected_result1[i][j]);

    Matrix<int, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<int, 7, 7> big_result = {{
        {16, 24, 32, 40, 48, 56, 64},
        {30, 44, 58, 72, 86, 100, 114},
        {14, 20, 26, 32, 38, 44, 50},
        {30, 44, 58, 72, 86, 100, 114},
        {2, 4, 6, 8, 10, 12, 14},
        {34, 52, 70, 88, 106, 124, 142},
        {72, 108, 144, 180, 216, 252, 288}
    }};

    auto result2 = big_one + big_one; 

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL(big_result[i][j], result2[i][j]);

}

void test_matrix_addition_float() {


    Matrix<float, 2, 7> mat1_1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<float, 2, 7> mat1_2 = {{
        {5,1,8,12,5,6,7},
        {3,4,5,6,6,6,9}
    }};

    Matrix<float, 2, 7> expected_result1 = {{
        {6,3,11,16,10,12,14},
        {6,8,10,12,13,14,18}
    }};

    auto result = mat1_1 + mat1_2;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_FLOAT(result[i][j], expected_result1[i][j]);

    Matrix<float, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<float, 7, 7> big_result = {{
        {16, 24, 32, 40, 48, 56, 64},
        {30, 44, 58, 72, 86, 100, 114},
        {14, 20, 26, 32, 38, 44, 50},
        {30, 44, 58, 72, 86, 100, 114},
        {2, 4, 6, 8, 10, 12, 14},
        {34, 52, 70, 88, 106, 124, 142},
        {72, 108, 144, 180, 216, 252, 288}
    }};

    auto result2 = big_one + big_one; 

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_FLOAT(big_result[i][j], result2[i][j]);

}

////////////////////////////////////////////////////////////
//                   Test Subtraction                     //
////////////////////////////////////////////////////////////



void test_matrix_subtraction_int() {


    Matrix<int, 2, 7> mat1_1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<int, 2, 7> mat1_2 = {{
        {5,1,8,12,5,6,7},
        {3,4,5,6,6,6,9}
    }};

    Matrix<int, 2, 7> expected_result1 = {{
        {-4, 1, -5, -8, 0, 0},
        {0, 0, 0, 0, 1, 2, 0}
    }};

    auto result = mat1_1 - mat1_2;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL(result[i][j], expected_result1[i][j]);

    Matrix<int, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<int, 7, 7> big_result = {{
        {16, 24, 32, 40, 48, 56, 64},
        {30, 44, 58, 72, 86, 100, 114},
        {14, 20, 26, 32, 38, 44, 50},
        {30, 44, 58, 72, 86, 100, 114},
        {2, 4, 6, 8, 10, 12, 14},
        {34, 52, 70, 88, 106, 124, 142},
        {72, 108, 144, 180, 216, 252, 288}
    }};

    auto result2 = big_result - big_one; 

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL(result2[i][j], big_one[i][j]);

}

void test_matrix_subtraction_float() {


    Matrix<float, 2, 7> mat1_1 = {{
        {1,2,3,4,5,6,7},
        {3,4,5,6,7,8,9}
    }};

    Matrix<float, 2, 7> mat1_2 = {{
        {5,1,8,12,5,6,7},
        {3,4,5,6,6,6,9}
    }};

    Matrix<float, 2, 7> expected_result1 = {{
        {-4, 1, -5, -8, 0, 0},
        {0, 0, 0, 0, 1, 2, 0}
    }};

    auto result = mat1_1 - mat1_2;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_FLOAT(result[i][j], expected_result1[i][j]);

    Matrix<float, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<float, 7, 7> big_result = {{
        {16, 24, 32, 40, 48, 56, 64},
        {30, 44, 58, 72, 86, 100, 114},
        {14, 20, 26, 32, 38, 44, 50},
        {30, 44, 58, 72, 86, 100, 114},
        {2, 4, 6, 8, 10, 12, 14},
        {34, 52, 70, 88, 106, 124, 142},
        {72, 108, 144, 180, 216, 252, 288}
    }};

    auto result2 = big_result - big_one; 

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_FLOAT(result2[i][j], big_one[i][j]);

}

////////////////////////////////////////////////////////////
//                 Test Transposition                     //
////////////////////////////////////////////////////////////

void test_transposition_single() {
    Matrix<int, 1, 1> small {{
        {6}
    }};

    Matrix<int, 1, 1> small_but_float {{
        {6}
    }};

    auto result = transpose(small);
    auto result_float = transpose(small_but_float);

    TEST_ASSERT_EQUAL_INT(small[0][0], result[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(small_but_float[0][0], result_float[0][0]);
}
void test_transposition_float() {
    Matrix<float, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<float, 7, 7> expected = {{
        {8, 15, 7, 15, 1, 17, 36},
        {12, 22, 10, 22, 2, 26, 54},
        {16, 29, 13, 29, 3, 35, 72},
        {20, 36, 16, 36, 4, 44, 90},
        {24, 43, 19, 43, 5, 53, 108},
        {28, 50, 22, 50, 6, 62, 126},
        {32, 57, 25, 57, 7, 71, 144}
    }};

    auto transposed = transpose(big_one);

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_FLOAT(expected[i][j], transposed[i][j]);
}

void test_transposition_int() {
    Matrix<int, 7, 7> big_one = {{
        {8, 12, 16, 20, 24, 28, 32},
        {15, 22, 29, 36, 43, 50, 57},
        {7, 10, 13, 16, 19, 22, 25},
        {15, 22, 29, 36, 43, 50, 57},
        {1, 2, 3, 4, 5, 6, 7},
        {17, 26, 35, 44, 53, 62, 71},
        {36, 54, 72, 90, 108, 126, 144}
    }};

    Matrix<int, 7, 7> expected = {{
        {8, 15, 7, 15, 1, 17, 36},
        {12, 22, 10, 22, 2, 26, 54},
        {16, 29, 13, 29, 3, 35, 72},
        {20, 36, 16, 36, 4, 44, 90},
        {24, 43, 19, 43, 5, 53, 108},
        {28, 50, 22, 50, 6, 62, 126},
        {32, 57, 25, 57, 7, 71, 144}
    }};

    auto transposed = transpose(big_one);

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            TEST_ASSERT_EQUAL_INT(expected[i][j], transposed[i][j]);
}

////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_matrix() {

    RUN_TEST(test_zero_matrix);

    RUN_TEST(test_matrix_float_equality);
    RUN_TEST(test_matrix_float_equality_edge_cases);

    RUN_TEST(test_matrix_int_equality);
    RUN_TEST(test_matrix_int_equality_edge_cases);

    RUN_TEST(test_matrix_multiplication_int);
    RUN_TEST(test_matrix_multiplication_float);

    RUN_TEST(test_matrix_addition_int);
    RUN_TEST(test_matrix_addition_float);

    RUN_TEST(test_matrix_subtraction_int);
    RUN_TEST(test_matrix_subtraction_float);

    RUN_TEST(test_transposition_single);
    RUN_TEST(test_transposition_int);
    RUN_TEST(test_transposition_float);

}
