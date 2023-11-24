#if (ARDUINO_SAMD_MKR1000)
#include <senseBoxIO.h>
#endif

#include "SenseBoxBLE.h"
#include <Adafruit_NeoPixel.h>

#include "gpx_data.h"    // Include the GPX data
#include "gpx_parser.h"  // Include the parsing functions

Adafruit_NeoPixel rgb_led_6 = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);

int period = 2000;
unsigned long time_now = 0;

int interval = 1;

float temperature = 0;
float humidity = 0;
float pm1 = 0;
float pm2_5 = 0;
float pm4 = 0;
float pm10 = 0;
float acc_x = 0;
float acc_y = 0;
float acc_z = 0;
float gps_lat = 0;
float gps_lng = 0;
float gps_spd = 0;
float dist_l = 0;

int temperatureCharacteristic = 0;
int humidityCharacteristic = 0;
int PMCharacteristic = 0;
int accelerationCharacteristic = 0;
int GPSCharacteristic = 0;
int distanceCharacteristic = 0;

size_t pos = 0;  // Initialize parsing position
double latitude;
double longitude;
double altitude;
char timestamp[20];  // Buffer to store timestamp

void receivedData();


void setup() {
  Serial.begin(115200);
  delay(500);
  SenseBoxBLE::start("senseBox-BLE");
  delay(1000);

  rgb_led_6.begin();

  rgb_led_6.setBrightness(30);


  SenseBoxBLE::configHandler = &receivedData;


  SenseBoxBLE::addService("CF06A218F68EE0BEAD048EBC1EB0BC84");

  temperatureCharacteristic = SenseBoxBLE::addCharacteristic("2CDF217435BEFDC44CA26FD173F8B3A8");
  humidityCharacteristic = SenseBoxBLE::addCharacteristic("772DF7EC8CDC4EA986AF410ABE0BA257");
  PMCharacteristic = SenseBoxBLE::addCharacteristic("7E14E07084EA489FB45AE1317364B979");
  accelerationCharacteristic = SenseBoxBLE::addCharacteristic("B944AF10F4954560968F2F0D18CAB522");
  GPSCharacteristic = SenseBoxBLE::addCharacteristic("8EDF8EBB12464329928DEE0C91DB2389");
  distanceCharacteristic = SenseBoxBLE::addCharacteristic("B3491B60C0F34306A30D49C91F37A62B");


  Serial.println(temperatureCharacteristic);
  Serial.println(humidityCharacteristic);
  Serial.println(PMCharacteristic);
  Serial.println(accelerationCharacteristic);
  Serial.println(GPSCharacteristic);
  Serial.println(distanceCharacteristic);


  int configCharacteristic = SenseBoxBLE::setConfigCharacteristic("29BD0A8551E44D3C914E126541EB2A5E", "60B1D5CE353944D2BB35FF2DAABE17FF");

  Serial.println("Config Characteristic");
  Serial.println(configCharacteristic);
}

void loop() {

  SenseBoxBLE::poll();

  // Your testing code here
  while (parseWaypoint(gpxData, pos, latitude, longitude, altitude, timestamp)) {
    SenseBoxBLE::poll();

    temperature = float(random(-5 * 1000, 40 * 1000)) / 1000.0;
    humidity = float(random(0 * 1000, 100 * 1000)) / 1000.0;
    pm1 = float(random(0 * 1000, 100 * 1000)) / 1000.0;
    pm2_5 = float(random(0 * 1000, 100 * 1000)) / 1000.0;
    pm4 = float(random(0 * 1000, 100 * 1000)) / 1000.0;
    pm10 = float(random(0 * 1000, 100 * 1000)) / 1000.0;
    acc_x = float(random(0 * 1000, 500 * 1000)) / 1000.0;
    acc_y = float(random(0 * 1000, 500 * 1000)) / 1000.0;
    acc_z = float(random(0 * 1000, 500 * 1000)) / 1000.0;
    gps_lat = float(random(-90 * 1000, 90 * 1000)) / 1000.0;
    gps_lng = float(random(-180 * 1000, 180 * 1000)) / 1000.0;
    gps_spd = float(random(0 * 1000, 50 * 1000)) / 1000.0;
    dist_l = float(random(0 * 1000, 400 * 1000)) / 1000.0;

    Serial.println(latitude);
    Serial.println(longitude);

    float myLat = latitude;
    float myLon = longitude;


    bool connected = SenseBoxBLE::write(temperatureCharacteristic, temperature);
    SenseBoxBLE::write(humidityCharacteristic, humidity);
    SenseBoxBLE::write(PMCharacteristic, pm1, pm2_5, pm4, pm10);
    SenseBoxBLE::write(accelerationCharacteristic, acc_x, acc_y, acc_z);
    SenseBoxBLE::write(GPSCharacteristic, myLat, myLon, gps_spd);
    SenseBoxBLE::write(distanceCharacteristic, dist_l);

    if (connected) {
      rgb_led_6.setPixelColor(0, rgb_led_6.Color(0, 255, 255));
      rgb_led_6.show();
    } else {
      rgb_led_6.setPixelColor(0, rgb_led_6.Color(0, 0, 0));
      rgb_led_6.show();
    }

    time_now = millis();


    while (millis() < time_now + period) {
      SenseBoxBLE::poll();
      delay(5);
    }
  }

  pos = 0;
}

void receivedData() {
  Serial.println("data:");
  uint8_t data[8];
  SenseBoxBLE::read(data, 8);

  uint8_t uploadInterval = data[0];
  interval = uploadInterval * 1000;

  bool useGPS = data[2];

  Serial.print("Upload Interval:\t");
  Serial.println(uploadInterval);
  Serial.print("Use GPS:\t\t");
  Serial.println(useGPS);
  Serial.print("...: ");
  Serial.println(data[4]);
  Serial.print("...: ");
  Serial.println(data[6]);
}
