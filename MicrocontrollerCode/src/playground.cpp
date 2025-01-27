
#include <Arduino.h>
// #include "common/drivers/sensor_bases/SensorBase.h"
#include "common/drivers/Icm20948.h"
#include <SPI.h>
#include "common/globals.h"
#include "common/math/vector.h"
#include "config/ConfigReader.h"

using namespace Cesium;
using namespace std;
using namespace Sensor;
#define LED 2

#define HSCK 14
#define HMISO 36
#define HMOSI 13
#define IMU_CS 33

Icm20948 sensor(IMU_CS, &SPI);


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

void setup() {
    
    Serial.begin(115200);
    SPI.begin(HSCK, HMISO, HMOSI, IMU_CS);

    sensor.setup();

    JsonDocument doc;
    File::json_open(vectors, doc);

    Vector3<int> test_int_vec;
    Vector3<float> test_float_vec;
    Vector3<int> expected = {{
        {1},
        {2},
        {3}
    }};
    Vector3<float> expected_float = {{
        {1.0},
        {2.0},
        {3.0}
    }};
    Serial.println(1);
    Serial.println("Good" + (String)File::json_extract<int,3,1>(doc, "good_vector", test_int_vec));
    Serial.println(2);
    Serial.println("Good" + (String)File::json_extract<float,3,1>(doc, "good_vector", test_float_vec));
    Serial.println(3);
    Serial.println(matrix_int_equals(expected, test_int_vec));
    Serial.println(4);
    Serial.println(matrix_float_equals(expected_float, test_float_vec));

}

void loop() {

    sensor.read();

    // Vector3<float> accel = sensor.get_accel_mps();
    // Vector3<float> gyro = sensor.get_w_rps();

    // Serial.print("Accel: " + String(norm(accel)));
    // Serial.print("\t\tValues: ");
    // for (auto i : accel) {
    //     Serial.print(i[0]);
    //     Serial.print("\t");
    // }

    // Serial.print("Norm: " + String(norm(gyro)));
    // Serial.print("\t\tGyro: ");
    // for (auto i : gyro) {
    //     Serial.print(i[0]);
    //     Serial.print("\t");
    // }

    // Serial.println();
    delay(10);

}
