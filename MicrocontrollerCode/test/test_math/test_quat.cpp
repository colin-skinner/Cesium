#include <unity.h>
#include <Arduino.h>

#include "common/math/quaternion.h"
using namespace std;



////////////////////////////////////////////////////////////
//              Test vec/quat conversion                  //
////////////////////////////////////////////////////////////

void test_vec_as_quat() {
    Vector3<float> test_vec = {1,1,1};
    Quaternion<float> expected = {0,1,1,1};
    Quaternion<float> NOT_expected = {1,2,2,2};
    auto result = vec_as_quat(test_vec);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(result[i][0], expected[i][0]);
    }
    TEST_ASSERT_TRUE(matrix_float_equals(result, expected));
    TEST_ASSERT_FALSE(matrix_float_equals(result, NOT_expected));
}

void test_quat_as_vec() {
    Quaternion<float> test_quat = {0,1,2,1};
    Vector3<float> expected = {1,2,1};
    Vector3<float> NOT_expected = {2,2,2};
    auto result = quat_as_vec(test_quat);


    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    TEST_ASSERT_TRUE(matrix_float_equals(result, expected));
    TEST_ASSERT_FALSE(matrix_float_equals(result, NOT_expected));
}

void test_quat_as_vec_edge() {
    Quaternion<float> test_quat = {2,1,2,1};
    Vector3<float> expected = {0,0,0};
    auto result = quat_as_vec(test_quat);

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }


}

////////////////////////////////////////////////////////////
//                 From Axis rotation                     //
////////////////////////////////////////////////////////////

void test_from_axis_rot_normal() {
    float angle_deg = 90;
    Vector3<float> axis = {0,5,0};

    Quaternion<float> expected = {std::cos(PI/4), 0, std::sin(PI/4), 0};
    Quaternion<float> not_expected = {std::cos(PI/2), 0, std::sin(PI/2), 0};

    Quaternion<float> result = quat_from_axis_rot<float>(angle_deg, axis);
    
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    // Tests to make sure it is equal to the expected quaternion
    TEST_ASSERT_TRUE(matrix_float_equals(result, expected));
    // Tests to make sure it is equal to the NOT expected quaternion
    TEST_ASSERT_FALSE(matrix_float_equals(result, not_expected));


    // NORM is 1
    TEST_ASSERT_EQUAL_FLOAT(1, norm(result));
}

void test_from_axis_rot_zero() {
    float angle_deg = 0;
    Vector3<float> axis = {0,5,1};

    Quaternion<float> expected = {1,0,0,0};
    Quaternion<float> not_expected = {0.5, 0.5, 0.5, 0.5};

    Quaternion<float> result = quat_from_axis_rot<float>(angle_deg, axis);
    
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    // Tests to make sure it is equal to the expected quaternion
    TEST_ASSERT_TRUE(matrix_float_equals(result, expected));
    // Tests to make sure it is equal to the NOT expected quaternion
    TEST_ASSERT_FALSE(matrix_float_equals(result, not_expected));


    // NORM is 1
    TEST_ASSERT_EQUAL_FLOAT(1, norm(result));
}

void test_from_axis_rot_random() {

    Quaternion<float> expected_quat = {0.336860, -0.1684304, 0, -0.9263671};

    float angle_deg = 140.6285398;
    Vector3<float> axis = {-0.1788854, 0, -0.9838699};

    auto result = quat_from_axis_rot<float>(angle_deg, axis);
    
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected_quat[i][0], result[i][0]);
    }
}


////////////////////////////////////////////////////////////
//                 To Axis rotation                     //
////////////////////////////////////////////////////////////


void test_axis_rotation_from_quat() {
    float expected_angle_deg = 45;
    Vector3<float> axis = {1,1,1};
    Vector3<float> unit_axis = unit(axis); //{0.5773502692, 0.5773502692, 0.5773502692};

    Quaternion<float> quat = {0.9238795325, 0.2209423827, 0.2209423827, 0.2209423827};
    Quaternion<float> bad_quat = {0.9238795325, 0.5309423827, 0.2209423827, 0.2209423827};

    Vector3<float> axis_rslt;
    float angle_rslt_deg;
    TEST_ASSERT_TRUE(axis_rot_from_quat(quat, angle_rslt_deg, axis_rslt));
    
    TEST_ASSERT_EQUAL_FLOAT(45., angle_rslt_deg);
    
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(unit_axis[i][0], axis_rslt[i][0]);
    }
    
    // Bad quat
    TEST_ASSERT_TRUE(axis_rot_from_quat(bad_quat, angle_rslt_deg, axis_rslt));

    TEST_ASSERT_EQUAL_FLOAT(67.3922501, angle_rslt_deg);
    TEST_ASSERT_FALSE(matrix_float_equals(unit_axis, axis_rslt));
}

void test_axis_rotation_from_quat_zero() {
    float expected_angle_deg = 0;
    Vector3<float> axis = {0,0,0};

    Quaternion<float> quat = {0.999,0.0001,0,0};

    Vector3<float> axis_rslt;
    float angle_rslt_deg;
    TEST_ASSERT_TRUE(axis_rot_from_quat(quat, angle_rslt_deg, axis_rslt));

    TEST_ASSERT_EQUAL_FLOAT(0., angle_rslt_deg);
    
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(0, axis_rslt[i][0]);
    }
}

// https://www.andre-gaschler.com/rotationconverter/
void test_axis_rotation_from_quat_random() {
    float expected_angle_deg = 140.6285398;
    Vector3<float> axis = {-0.1788854, 0, -0.9838699};

    Quaternion<float> quat = {0.336860, -0.1684304, 0, -0.9263671};

    Vector3<float> axis_rslt;
    float angle_rslt_deg;
    TEST_ASSERT_TRUE(axis_rot_from_quat(quat, angle_rslt_deg, axis_rslt));
    
    TEST_ASSERT_EQUAL_FLOAT(expected_angle_deg, angle_rslt_deg);
    
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(axis[i][0], axis_rslt[i][0]);
    }
}

////////////////////////////////////////////////////////////
//                     R_from_quat                        //
////////////////////////////////////////////////////////////

void test_R_from_quat_identity() {
    Quaternion<float> I_quat = {1,0,0,0};

    Matrix3<float> I_mat = {{{1,0,0},
                             {0,1,0},
                             {0,0,1}}};

    auto result = R_from_quat(I_quat);

    TEST_ASSERT_TRUE(matrix_float_equals(I_mat, result));
}

void test_R_from_quat_non_identity() {
    Quaternion<float> I_quat = {1,0,0,0};

    Matrix3<float> non_I_mat = {{{1,0.1,0},
                             {0,1,0},
                             {0,0,1}}};

    auto result = R_from_quat(I_quat);

    TEST_ASSERT_FALSE(matrix_float_equals(non_I_mat, result));
}

// https://www.andre-gaschler.com/rotationconverter/
void test_R_from_quat_random() {
    Quaternion<float> quat = {0.3368608, -0.1684304, 0, -0.9263671};

    Matrix3<float> mat = {{{-0.7163121,  0.6241135,  0.3120568},
                           {-0.6241135, -0.7730497,  0.1134752},
                           {0.3120568, -0.1134752,  0.9432624}}};

    auto result = R_from_quat(quat);

    TEST_ASSERT_TRUE(matrix_float_equals(mat, result));
}


////////////////////////////////////////////////////////////
//                     quat_from_R                        //
////////////////////////////////////////////////////////////

void test_quat_from_R_identity() {
    Quaternion<float> I_quat = {1,0,0,0};

    Matrix3<float> I_mat = {{{1,0,0},
                             {0,1,0},
                             {0,0,1}}};

    auto result = quat_from_R(I_mat);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(I_quat[i][0], result[i][0]);
    }
}

void test_quat_from_R_non_identity() {
    Quaternion<float> non_I_quat = {1,0,0,0};

    Matrix3<float> non_I_mat = {{{1,0.1,0},
                                 {0,1,0},
                                 {0,0,1}}};

    auto result = quat_from_R(non_I_mat);

    TEST_ASSERT_FALSE(matrix_float_equals(non_I_quat, result));
}

void test_quat_from_R_random() {
    Quaternion<float> quat = {0.3368608, -0.1684304, 0, -0.9263671};

    Matrix3<float> mat = {{{-0.7163121,  0.6241135,  0.3120568},
                           {-0.6241135, -0.7730497,  0.1134752},
                           {0.3120568, -0.1134752,  0.9432624}}};

    auto result = quat_from_R(mat);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(quat[i][0], result[i][0]);
    }
}

////////////////////////////////////////////////////////////
//                      quat_mulk                         //
////////////////////////////////////////////////////////////

// https://www.omnicalculator.com/math/quaternion
void test_quat_mult_random() {
    Quaternion<float> q1 = {0.1, 0.1, 0.1, 0.2};
    Quaternion<float> q2 = {0.1, 0.2, 0.3, 0.4};
    Quaternion<float> expected = {-0.12, 0.01, 0.04, 0.07};

    // One way
    auto result = quat_mult(q1, q2);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    // Other way
    expected = {-0.12, 0.05, 0.04, 0.05};
    result = quat_mult(q2, q1);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }
}

void test_quat_mult_zero() {
    Quaternion<float> q1 = {0.0, 0.1, 0.1, 0.2};
    Quaternion<float> q2 = {0.1, 0.0, 0.0, 0.0};
    Quaternion<float> expected = {0, 0.01, 0.01, 0.02};

    // One way
    auto result = quat_mult(q1, q2);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    // Other way
    result = quat_mult(q2, q1);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }
}

void test_quat_mult_mega_random() {
    Quaternion<float> q1 = {5.2, 2.78, 3.235, 134.5235};
    Quaternion<float> q2 = {0.1, 0.2131234, 0.1231, 19.0};
    Quaternion<float> expected = {-2556.417211552, 46.29139883, -23.1862743001, 111.905113801};

    // One way
    auto result = quat_mult(q1, q2);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }

    // Other way
    expected = {-2556.417211552, -43.51891547, 25.1135143001, 112.599586199};
    result = quat_mult(q2, q1);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }
}

////////////////////////////////////////////////////////////
//                        Inverse                         //
////////////////////////////////////////////////////////////

void test_quat_inv() {
    Quaternion<float> quat = {-2556.417211552, 46.29139883, -23.1862743001, 111.905113801};
    Quaternion<float> expected = {-2556.417211552, -46.29139883, 23.1862743001, -111.905113801};

    auto result = inv(quat);

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }
}

////////////////////////////////////////////////////////////
//                         Apply                          //
////////////////////////////////////////////////////////////

void test_apply_identity() {
    Quaternion<float> I_quat = {1,0,0,0};
    Vector3<float> start = {4,5,6};

    auto result = quat_apply(I_quat, start);

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(start[i][0], result[i][0]);
    }
}

// Best site ever: https://www.vcalc.com/wiki/vector-rotation
void test_apply(Vector3<float> start, Vector3<float> expected, float angle, Vector3<float> axis) {
    Quaternion<float> I_quat = quat_from_axis_rot<float>(angle, axis);

    auto result = quat_apply(I_quat, start);

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_FLOAT(expected[i][0], result[i][0]);
    }
}

void test_apply_x() {
    test_apply({4,5,6}, {4, 5, 6}, 0, {1,0,0}); // 0
    test_apply({4,5,6}, {4, -6, 5}, 90, {1,0,0}); // 90
    test_apply({4,5,6}, {4, -5, -6}, 180, {1,0,0}); // 180
    test_apply({4,5,6}, {4, 2.3233,7.45669}, 22.5, {1,0,0}); // 22.5
}

void test_apply_random() {
    Vector3<float> axis = {0.444, 0.235, 0.134};
    
    test_apply({4.246, 212.14, 3.32423},
        {32.53964,43.62128,205.11171},
        90,
        unit(axis)
    );

    test_apply({1.234, 2.345, 3.456},
        {4.16581,-0.57947,-1.1296},
        157,
        unit(axis)
    );

}



////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void test_all_quat() {

    RUN_TEST(test_vec_as_quat);
    RUN_TEST(test_quat_as_vec);
    RUN_TEST(test_quat_as_vec_edge);

    RUN_TEST(test_from_axis_rot_normal);
    RUN_TEST(test_from_axis_rot_zero);
    RUN_TEST(test_from_axis_rot_random);

    RUN_TEST(test_axis_rotation_from_quat);
    RUN_TEST(test_axis_rotation_from_quat_zero);
    RUN_TEST(test_axis_rotation_from_quat_random);

    RUN_TEST(test_R_from_quat_identity);
    RUN_TEST(test_R_from_quat_non_identity);
    RUN_TEST(test_R_from_quat_random);

    RUN_TEST(test_quat_from_R_identity);
    RUN_TEST(test_quat_from_R_non_identity);
    RUN_TEST(test_quat_from_R_random);

    RUN_TEST(test_quat_mult_random);
    RUN_TEST(test_quat_mult_zero);
    RUN_TEST(test_quat_mult_mega_random);

    RUN_TEST(test_quat_inv);

    RUN_TEST(test_apply_identity);
    RUN_TEST(test_apply_x);
    RUN_TEST(test_apply_random);

    
}

