// Automatically generated document
// Generate this code with 'create_config_header.py'
#pragma once

namespace Cesium {
namespace Config {

const char imu_1_config[] = R"(
{
    "acceleration_bias": [0.01, -0.02, 0.03],
    "gyroscope_bias": [0.001, -0.002, 0.003],
    "body_to_sensor": [
        [1.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 0, 1.0]
        ],
    "cool_vector": [
        [1],
        [2],
        [3]
    ],
    "name": "luke"
}
)";


const char test_config[] = R"(
{
    "string": "file",
    "int": 1,
    "float": 9.999,
    "int_vector": [
        [4],
        [5],
        [7]
    ],
    "float_vector": [
        [4.5],
        [5.6],
        [7.8]
    ],
    "int_array": [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ],
    "float_array": [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]
}
)";


const char imu_2_config[] = R"(
{
    "acceleration_bias": [0.01, -0.02, 0.03],
    "gyroscope_bias": [0.001, -0.002, 0.003],
    "transformation_matrix": [
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
        ],
    "name": "leia"
}
)";



} // namespace Cesium
} // namespace Config
