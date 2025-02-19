//TODO: Make config reader
#include <ArduinoJson.h>
#include "all_configs.h"
#include <unordered_map>
#include "../common/math/vector.h"

namespace Cesium {
namespace File {

// TODO: unit test

// Read in document to JsonDocument object

bool json_open(const char* config_name, JsonDocument& doc) {
    
    const size_t json_size = strlen(config_name);

    DeserializationError error = deserializeJson(doc, config_name);

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.f_str());
        return false;
    }

    return true;
}

template <typename T>
bool json_extract(JsonDocument doc, const char* name, T& value) {

    // RETURN_FALSE_IF_FALSE(doc[name].is<T>()); 

    // value = doc[name].as<T>();
    return true;
}

template <typename T, int rows, int cols>
bool json_extract(JsonDocument doc, const char* name, Matrix<T, rows, cols>& value) {
    // JsonObject json_obj = doc.as<JsonObject>();

    // JsonArray matrix = json_obj[name].as<JsonArray>();

    // // Return if number of rows doesn't match
    // Serial.println(matrix.size());
    // RETURN_FALSE_IF_FALSE(matrix.size() == rows);
    // for (size_t i = 0; i < rows; i++) {
        
    //     // Return if not an array, or number of cols doesn't match
    //     RETURN_FALSE_IF_FALSE(matrix[i].is<JsonArray>());
    //     Serial.print(matrix[i].size());
    //     RETURN_FALSE_IF_FALSE(matrix[i].size() == cols);
    //     for (size_t j = 0; j < cols; j++) {
    //         // Return if not datatype
    //         RETURN_FALSE_IF_FALSE(matrix[i][j].is<T>());
    //         value[i][j] = matrix[i][j].as<T>();
    //     }
    // }

    return true;
}

// template <typename T, int rows>
// bool json_extract(JsonDocument doc, const char* name, Vector<T, rows>& value) {
//     RETURN_FALSE_IF_FALSE(json_extract(doc, name, value));
// }
}
}

