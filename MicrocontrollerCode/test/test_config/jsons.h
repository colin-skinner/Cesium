const char good_json[] = R"(
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

const char bad_json[] = R"(
{
    "string": "file",
    "int": 1,  YAYDADADA THIS IS A BAD JSON
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

const char vectors[] = R"(
{
    "good_vector" : [
        [1],
        [2],
        [3]
    ],
    "bad_col_size" : [
        [1,2],
        [2],
        [3]
    ],
    "row_mismatch" : [
        [1],
        [2]
    ],
    "bad_datatype" : [
        ["wow"],
        [2],
        [3]
    ]

}
)";