#pragma once

#include <array>
#include <Arduino.h>
#include "../globals.h"

// Defining NxM matrix
// Example call: Matrix<float, 3, 3> mat3x3 = {{{4, 7, 1}, {2, 6, 0}, {0, 5, 0}}};
template <typename T, size_t row, size_t col>
using Matrix = std::array<std::array<T, col>, row>;

template <typename T>
using Matrix3 = std::array<std::array<T, 3>, 3>;


// MATRIX MATH WOOO

// Function to transpose a matrix
template <typename T, size_t row, size_t col>
Matrix<T, col, row> transpose(const Matrix<T, row, col>& mat) {
    Matrix<T, col, row> result;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            result[j][i] = mat[i][j];
        }
    }
    return result;
}

// Overload float matrix equality
template <size_t row, size_t col>
bool matrix_float_equals(const Matrix<float, row, col>& mat1, const Matrix<float, row, col>& mat2) {

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            if (abs(mat1[i][j] - mat2[i][j]) > 1e-6) {
                return false;
            }
        }
    }
    return true;
}

// Overload float matrix equality
template <size_t row, size_t col>
bool matrix_int_equals(const Matrix<int, row, col>& mat1, const Matrix<int, row, col>& mat2) {

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            if (mat1[i][j] != mat2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Overload operator* for matrix multiplication
template <typename T, size_t row_f, size_t dim_int, size_t col_f>
Matrix<T, row_f, col_f> operator*(const Matrix<T, row_f, dim_int>& mat1, const Matrix<T, dim_int, col_f>& mat2) {
    Matrix<T, row_f, col_f> result;
    for (size_t i = 0; i < row_f; i++) {
        for (size_t j = 0; j < col_f; j++) {

            result[i][j] = 0;
            for (size_t k = 0; k < dim_int; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}

// Overload operator* for matrix multiplication
template <typename T, size_t row, size_t col>
Matrix<T, row, col> operator*(const Matrix<T, row, col>& mat, T factor) {
    Matrix<T, row, col> result{};
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            result[i][j] = mat[i][j] * factor;
        }
    }
    return mat;
}

// Overload the + operator for Matrix addition
template <typename T, size_t row, size_t col>
Matrix<T, row, col> operator+(const Matrix<T, row, col>& lhs, const Matrix<T, row, col>& rhs) {
    Matrix<T, row, col> result;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}

// Overload the - operator for Matrix subtraction
template <typename T, size_t row, size_t col>
Matrix<T, row, col> operator-(const Matrix<T, row, col>& lhs, const Matrix<T, row, col>& rhs) {
    Matrix<T, row, col> result;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}

// Helper function to print a Matrix
template <typename T, size_t row, size_t col>
void printMatrix(const Matrix<T, row, col>& matrix) {
    for (const auto& r : matrix) {
        for (const auto& val : r) {
            DEBUG(String(val, 5));
            DEBUG(" ");
        }
        DEBUGLN();
    }
}


