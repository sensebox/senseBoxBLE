#include <SenseBoxMCU.h>
#include "SenseBoxBLE.h"

void receivedData();

void setup() {
  Serial.begin(115200);
  delay(500);
  SenseBoxBLE::start("senseBox-BLE");
  delay(1000);

  SenseBoxBLE::configHandler = &receivedData;

  int configCharacteristic = SenseBoxBLE::setConfigCharacteristic("29BD0A8551E44D3C914E126541EB2A5E", "60B1D5CE353944D2BB35FF2DAABE17FF");

  Serial.println("Config Characteristic");
  Serial.println(configCharacteristic);
}

void loop() {

  SenseBoxBLE::poll();

  delay(5);
}

void receivedData() {
  Serial.println("data:");
  uint8_t data[8];
  SenseBoxBLE::read(data, 8);

  Serial.print("0:\t");
  Serial.println(data[0]);
  Serial.print("1:\t");
  Serial.println(data[2]);
  Serial.print("2:\t");
  Serial.println(data[4]);
  Serial.print("3:\t");
  Serial.println(data[6]);
}
