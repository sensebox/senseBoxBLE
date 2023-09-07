#if defined(ARDUINO_SAMD_MKR1000)

#include "Arduino.h"
#include <stdio.h>
#include "SenseBoxBLE.h"

uint16_t SenseBoxBLE::minConInterval = 12;  //7.5ms
uint16_t SenseBoxBLE::maxConInterval = 48; //30ms
uint16_t SenseBoxBLE::slaveLatency = 0;
uint16_t SenseBoxBLE::timeout = 50;

uint8_t* SenseBoxBLE::data = nullptr; //this pointer points to the data the user wants to write in the characteristic

void(*SenseBoxBLE::configHandler)() = nullptr;

void SenseBoxBLE::start(const char* DEVICE_NAME)
{
  port.begin();
  port.stopAdvertise();

  port.checkResponse("AT+UBTAD=020A0605121800280011074A5153BA405E438B7146F7300100DFCD",1000);

  // set connection parameter
  port.setConnectionInterval(minConInterval, maxConInterval);

  port.checkResponse("AT+UBTAD=020A0605121800280011074A5153BA405E438B7146F7300100DFCD",1000);


  port.advertise();
  port.setLocalName(DEVICE_NAME);
}

void SenseBoxBLE::start() {
    SenseBoxBLE::start("senseBox-BLE");
}

bool SenseBoxBLE::advertise(){
  return port.advertise();
}

bool SenseBoxBLE::stopAdvertise(){
  return port.stopAdvertise();
}

int SenseBoxBLE::addService(const char* serviceUUID){
  return port.parseResponse(String("AT+UBTGSER=")+serviceUUID,1000);
}

int SenseBoxBLE::addCharacteristic(const char* characteristicUUID, int properties, int permissions, int maxLen){
  return port.parseResponse(String("AT+UBTGCHA=")+characteristicUUID+",1a,1,1",1000);
}

void SenseBoxBLE::poll(int timeout)
{
  auto starttime=millis();
  while(millis()-starttime<timeout){
    poll();
    delay(5);
  }
}

void SenseBoxBLE::poll(){
    if(port.poll()){
        port.flushInput();
    }

}

bool SenseBoxBLE::write(int characteristic, float& value)
{
  data = reinterpret_cast<uint8_t*>(&value);
  port.writeValue(characteristic,data,4);
}

bool SenseBoxBLE::write(int characteristic, float& f1, float& f2)
{
  float array[2] = {f1, f2};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,8);
}

bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3)
{
  float array[3] = {f1, f2, f3};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,12);
}

bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3 , float& f4)
{
  float array[4] = {f1, f2, f3, f4};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,16);
}

bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3 , float& f4, float& f5)
{
  float array[5] = {f1, f2, f3, f4, f5};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,20);
}

#endif