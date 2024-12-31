#include "matrix.h"

// Defining vertical vector
template <typename T, int N>
using VectorN = Matrix<T, N, 1>;

// Defining 3 dimensional vector
template <typename T>
using Vector3 = VectorN<T, 3>;  

