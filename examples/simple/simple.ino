#include <SenseBoxMCU.h>
#include <SenseBoxBLE.h>

float value = 0;
int tempCharacteristic = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
  SenseBoxBLE::start("senseBox-BLE");
  SenseBoxBLE::addService("CF06A218F68EE0BEAD048EBC1EB0BC84");
  tempCharacteristic = SenseBoxBLE::addCharacteristic("2CDF217435BEFDC44CA26FD173F8B3A8");
}

void loop() {

  SenseBoxBLE::poll();

  value = float(random(0, 20000)) / 1000.0;

  Serial.print("Characteristic handle ");
  Serial.print(tempCharacteristic);
  Serial.print(" ");
  Serial.print(value);

  bool temprestul = SenseBoxBLE::write(tempCharacteristic, value);

  Serial.print(" ");
  Serial.println(temprestul);

  delay(1000);
}
