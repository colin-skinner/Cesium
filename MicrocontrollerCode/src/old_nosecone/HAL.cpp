#include "HAL.h"

namespace Cesium {

    SPIClass hspi(HSPI);
    SPIClass vspi(VSPI);
    CanBus can_bus(CAN_RX, CAN_TX);
    FileSystem filesystem;
    Sensor::Ms5607 altimeter2(ALTIMETER2_CS, &vspi);
    Sensor::Bmp388 altimeter1(ALTIMETER1_CS, &vspi);
    Sensor::Icm20948 imu2(IMU2_CS, &vspi);
    Sensor::Bmi323 imu1(IMU1_CS, &vspi);
    
    void init_cs_pins() {
        pinMode(IMU1_CS, OUTPUT);
        pinMode(IMU2_CS, OUTPUT);
        pinMode(ALTIMETER1_CS, OUTPUT);
        pinMode(ALTIMETER2_CS, OUTPUT);
        pinMode(SD_CS, OUTPUT);
        // pinMode(TRANSMITTER_CS, OUTPUT);

        digitalWrite(IMU1_CS, HIGH);
        digitalWrite(IMU2_CS, HIGH);
        digitalWrite(ALTIMETER1_CS, HIGH);
        digitalWrite(ALTIMETER2_CS, HIGH);
        digitalWrite(SD_CS, HIGH);
        // digitalWrite(TRANSMITTER_CS, HIGH);
    }

    void init_interfaces() {
        hspi.begin(HSCK, HMISO, HMOSI);
        vspi.begin(VSCK, VMISO, VMOSI);
        can_bus.setup();
    }   

    void init_sensors() {
        imu1.setup();
        imu2.setup();
        altimeter2.setup();
    }

}
