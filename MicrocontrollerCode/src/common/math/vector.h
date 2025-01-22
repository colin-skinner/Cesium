#pragma once

#include "matrix.h"

// Defining vertical vector
template <typename T, int N>
using VectorN = Matrix<T, N, 1>;

// Defining 3 dimensional vector
template <typename T>
using Vector3 = VectorN<T, 3>;  


// Function to dot product a vector
template <typename T, size_t rows>
VectorN<T, rows> dot(const VectorN<T, rows>& vec1, const VectorN<T, rows>& vec2) {
    VectorN<T, rows> result;
    for (size_t i = 0; i < rows; i++) {
        result[i][0] = vec1[i][0] * vec2[i][0];
    }
    return result;
}

// Function to cross product a vector
template <typename T>
Vector3<T> cross(const Vector3<T>& vec1, const Vector3<T>& vec2) {
    Matrix<T, 3, 3> decomposed_vec1 = {{
        {0, -vec1[2][0], vec1[1][0]},
        {vec1[2][0], 0, -vec1[0][0]},
        {-vec1[1][0], vec1[0][0], 0},
    }};

    return decomposed_vec1 * vec2;
}

