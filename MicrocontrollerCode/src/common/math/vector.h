#pragma once

#include "matrix.h"

// Defining vertical vector
template <typename T, int N>
using Vector = Matrix<T, N, 1>;

// Defining 3 dimensional vector
template <typename T>
using Vector3 = Vector<T, 3>;  


// Function to dot product a vector
template <typename T, size_t rows>
Vector<T, rows> dot(const Vector<T, rows>& vec1, const Vector<T, rows>& vec2) {
    Vector<T, rows> result;
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

// Norm (TODO: test)
template <typename T>
T norm(const Vector3<T>& vec) {
    Vector3<T> rslt = dot(vec, vec);
    return sqrt(rslt[0][0] + rslt[0][1] + rslt[0][2]); 
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