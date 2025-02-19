#pragma once

#include "matrix.h"

// Defining vertical vector
template <typename T, int N>
using Vector = Matrix<T, N, 1>;

// Defining 3 dimensional vector
template <typename T>
using Vector3 = Vector<T, 3>;  


// Function to dot product a vector TODO: fix this this is so damn wrong
template <typename T, size_t rows>
T dot(const Vector<T, rows>& vec1, const Vector<T, rows>& vec2) {
    T sum = 0;
    for (size_t i = 0; i < rows; i++) {
        sum += vec1[i][0] * vec2[i][0];
    }
    return sum;
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

// Norm (TODO: test)
template <typename T, size_t N>
T norm(const Vector<T, N>& vec) {
    T dot_product = dot(vec, vec);

    return sqrt(dot_product); 
}

// TODO: unit
// Function to unit-ize a vector
template <typename T, size_t N>
Vector<T, N> unit(const Vector<T, N>& vec1) {
    T length = norm(vec1);
    Vector<T, N> result = vec1;
    
    for (auto& row : result) {
        row[0] /= length;
    }

    return result;
}

// Copying this to a vector of bytes. Returns number of bytes advanced. TODO: test
template <typename T, size_t N>
size_t vec_to_buffer(const Vector<T, N>& vec, std::vector<uint8_t>& buffer, size_t start) {

    size_t index = start;
    // For each row in the vector, copy the 
    for (const auto& row : vec) {
        
        // Cast to uint8_t pointer
        const uint8_t* src = reinterpret_cast<const uint8_t*>(&row[0]);

        // Ensure the vector has enough space
        if (index + sizeof(T) > buffer.size()) {
            buffer.resize(index + sizeof(T)); 
        }

        // Copy sizeof(T) bytes
        memcpy(&buffer[index], src, sizeof(T)); 
        index += sizeof(T); // Advance index
    }

    return index - start;
}