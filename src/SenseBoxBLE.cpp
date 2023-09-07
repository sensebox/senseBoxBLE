#if defined(ARDUINO_SAMD_MKR1000)

#include "NINAB31serial.h"
#include "Arduino.h"
#include <stdio.h>
#include "SenseBoxBLE.h"
/*
BLEService SenseBoxBLE::phyphoxExperimentService{SenseBoxBLEExperimentServiceUUID}; // create service
BLECharacteristic SenseBoxBLE::experimentCharacteristic{SenseBoxBLEExperimentCharacteristicUUID, BLERead | BLEWrite| BLENotify, 20, false};
BLECharacteristic SenseBoxBLE::controlCharacteristic{SenseBoxBLEExperimentControlCharacteristicUUID, BLERead | BLEWrite| BLENotify, 20, false};

BLEService SenseBoxBLE::phyphoxDataService{SenseBoxBLEDataServiceUUID}; // create service
BLECharacteristic SenseBoxBLE::dataCharacteristic{SenseBoxBLEDataCharacteristicUUID, BLERead | BLEWrite | BLENotify, 20, false};
BLECharacteristic SenseBoxBLE::configCharacteristic{SenseBoxBLEConfigCharacteristicUUID, BLERead | BLEWrite| BLENotify, 20, false};
*/
uint16_t SenseBoxBLE::minConInterval = 12;  //7.5ms
uint16_t SenseBoxBLE::maxConInterval = 48; //30ms
uint16_t SenseBoxBLE::slaveLatency = 0;
uint16_t SenseBoxBLE::timeout = 50;

uint16_t SenseBoxBLE::MTU = 20;
// uint16_t SenseBoxBLEExperiment::MTU = 20;

int SenseBoxBLE::h_service=0;
int SenseBoxBLE::h_characteristic=0;

uint8_t* SenseBoxBLE::data = nullptr; //this pointer points to the data the user wants to write in the characteristic
uint8_t* SenseBoxBLE::p_exp = nullptr; //this pointer will point to the byte array which holds an experiment

size_t SenseBoxBLE::expLen = 0; //try o avoid this maybe use std::array or std::vector
uint8_t SenseBoxBLE::EXPARRAY[4000] = {0};// block some storage

uint8_t SenseBoxBLE::controlCharValue[21]={0};
uint8_t SenseBoxBLE::configCharValue[21]={0};



void(*SenseBoxBLE::configHandler)() = nullptr;

void SenseBoxBLE::start(const char* DEVICE_NAME, uint8_t* exp_pointer, size_t len){
  p_exp = exp_pointer;
  expLen = len;
  start(DEVICE_NAME);
}

void SenseBoxBLE::start(uint8_t* exp_pointer, size_t len){
  p_exp = exp_pointer;
  expLen = len;
  start();
}

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
        // if(parseValue(controlCharValue,port.checkCharWritten(h_controlCharacteristic))){
        //     controlCharacteristicWritten();
        // }
        // if(parseValue(configCharValue,port.checkCharWritten(h_configCharacteristic))){
        //     configCharacteristicWritten();
        // }
        port.flushInput();
    }

}

void SenseBoxBLE::read(uint8_t *arrayPointer, unsigned int arraySize)
{
  memcpy(arrayPointer, configCharValue, arraySize);
  //configCharacteristic.readValue(arrayPointer, arraySize);
}

void SenseBoxBLE::read(float& f)
{
  //uint8_t readDATA[4];
  //configCharacteristic.readValue(readDATA, 4);
  memcpy(&f,configCharValue,4);
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


bool parseValue(uint8_t* target, String s)
{
    String hextable="0123456789ABCDEF";
    if(s.length() && s.length()<41){
        s.toUpperCase();
        for(int i=0;i<s.length();i+=2){
            int highbits=hextable.indexOf(s[i]);
            int lowbits=hextable.indexOf(s[i+1]);
            if(highbits==-1 || lowbits==-1){
                return false;
            }
            target[i]=(((highbits<<4)|lowbits)&0xff);
        }
        return true;
    }
    return false;
}
#endif