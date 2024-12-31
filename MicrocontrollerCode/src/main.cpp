#include <Arduino.h>
// #include <SPI.h>
#include <EEPROM.h>
#include "common/drivers/icm20948.h"
#include "common/drivers/ina233.h"
#include "common/drivers/ms5611.h"
#include "common/globals.h"

uint8_t icm_pin = 0;
auto icm = Cesium::Icm20948(icm_pin, &SPI);
// auto gps = Cesium::NeoM8U();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  DEBUGLN("HEY");
  delay(1000);
}
