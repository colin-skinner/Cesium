
#include <Arduino.h>
// // #include "common/drivers/sensor_bases/SensorBase.h"
// // #include "common/drivers/Icm20948.h"
// // #include <SPI.h>
// // #include "common/globals.h"
// // #include "common/math/vector.h"
// // #include "config/ConfigReader.h"
// #include "common/drivers/Ms5607.h"
// #include "common/drivers/sensor_bases/BarometerBase.h"
// #include "common/drivers/Bmi323.h"
// #include "common/drivers/Bmp388.h"
// #include <CAN.h>

// // String type = "receiver";
// // String type = "sender";

// // uint8_t i;


// #define HSCK 14
// #define HMISO 36
// #define HMOSI 13
// #define IMU_CS 33

// #define VSCK 18
// #define VMISO 19
// #define VMOSI 23
// #define MS5607_CS 2
// #define BMP_CS 32
// #define BMI_CS 16

// SPIClass vspi(VSPI);
// SPIClass hspi(HSPI);

// // Cesium::Sensor::Ms5607 altimeter2(MS5607_CS, &vspi);

// // Cesium::Sensor::Bmi323 bmi(BMI_CS, &vspi);

// Cesium::Sensor::Bmp388 altimeter1(BMP_CS, &hspi);


// void setup() {

//     pinMode(MS5607_CS, OUTPUT);
//     pinMode(IMU_CS, OUTPUT);
//     pinMode(BMI_CS, OUTPUT);
//     pinMode(4, OUTPUT);
//     pinMode(BMP_CS, OUTPUT);

//     digitalWrite(MS5607_CS, HIGH);
//     digitalWrite(IMU_CS, HIGH);
//     digitalWrite(BMI_CS, HIGH);
//     digitalWrite(4, HIGH);
//     digitalWrite(BMP_CS, HIGH);
    
    
//     Serial.begin(115200);

//     vspi.begin(VSCK, VMISO, VMOSI);
//     hspi.begin(HSCK, HMISO, HMOSI);

//     altimeter1.setup();
    
//     // altimeter2.setup();

    
//     // // CAN.setPins(26, 25);
//     // CAN.setPins(25, 26);
    
//     // if (!CAN.begin(500E3)) {
//     //     Serial.println("Starting CAN failed!");
//     //     while (1);
//     // }

//     // i = 0;

    



// }

// void loop() {

//     // altimeter2.read();
//     // bmi.read();
//     // altimeter1.read();
//     // if (type == "sender") {
//     //     Serial.print("Sending packet ... ");

//     //     CAN.beginPacket(0x12);
//     //     // CAN.write('h');
//     //     // CAN.write('e');
//     //     // CAN.write('l');
//     //     // CAN.write('l');
//     //     // CAN.write('o');
//     //     CAN.write(i);
//     //     CAN.endPacket();

//     //     Serial.println("done");

//     //     delay(100);
//     //     i++;
//     // }

//     // else if (type == "receiver") {
        
//     //     // try to parse packet
//     //     int packetSize = CAN.parsePacket();

//     //     if (packetSize) {
//     //         // received a packet
//     //         Serial.print("Received ");

//     //         if (CAN.packetExtended()) {
//     //             Serial.print("extended ");
//     //         }

//     //         if (CAN.packetRtr()) {
//     //             // Remote transmission request, packet contains no data
//     //             Serial.print("RTR ");
//     //         }

//     //         Serial.print("packet with id 0x");
//     //         Serial.print(CAN.packetId(), HEX);

//     //         if (CAN.packetRtr()) {
//     //             Serial.print(" and requested length ");
//     //             Serial.println(CAN.packetDlc());
//     //         } 
//     //         else {
//     //             Serial.print(" and length ");
//     //             Serial.println(packetSize);

//     //             // only print packet data for non-RTR packets
//     //             while (CAN.available()) {
//     //                 Serial.print((int)CAN.read());
//     //             }
//     //             Serial.println();
//     //         }

//     //         Serial.println();
//     //     }
//     // }


    

// }


