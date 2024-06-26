//#if defined(ARDUINO_SAMD_MKR1000)
//#if defined(SENSEBOX_MCU_S2)

#include "Arduino.h"
#include <stdio.h>
#include "SenseBoxBLE.h"

uint16_t SenseBoxBLE::minConInterval = 12;  //7.5ms
uint16_t SenseBoxBLE::maxConInterval = 48; //30ms
uint16_t SenseBoxBLE::slaveLatency = 0;
uint16_t SenseBoxBLE::timeout = 50;

int SenseBoxBLE::h_configCharacteristic=0;

uint8_t* SenseBoxBLE::data = nullptr; //this pointer points to the data the user wants to write in the characteristic

uint8_t SenseBoxBLE::configCharValue[21]={0};

void(*SenseBoxBLE::configHandler)() = nullptr;

/**
  * @brief Starts the BLE module and sets the device name.
  *
  * @param DEVICE_NAME The name of the device to be advertised.
  */
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

/**
  * @brief Starts the BLE module with the default device name "senseBox-BLE".
  *
  */
void SenseBoxBLE::start() {
    SenseBoxBLE::start("senseBox-BLE");
}

/**
  * @brief Advertises the device.
  *
  * @return true if advertising was successful, false otherwise.
  */
bool SenseBoxBLE::advertise(){
  return port.advertise();
}

 /**
  * @brief Stops advertising the device.
  *
  * @return true if advertising was stopped successfully, false otherwise.
  */
bool SenseBoxBLE::stopAdvertise(){
  return port.stopAdvertise();
}

/**
  * @brief Adds a service to the BLE module.
  *
  * @param serviceUUID The UUID of the service to be added.
  * @return The ID of the service if successful, -1 otherwise.
  */
int SenseBoxBLE::addService(const char* serviceUUID){
  return port.parseResponse(String("AT+UBTGSER=")+serviceUUID,1000);
}

void SenseBoxBLE::setName(String name){
  port.setLocalName(name);
}

String SenseBoxBLE::getMCUId(){
  return port.parseStringResponse("AT+UMLA=1",1000);
}

/**
  * @brief Adds a characteristic to the BLE module.
  *
  * @param characteristicUUID The UUID of the characteristic to be added.
  * @return The ID of the characteristic if successful, -1 otherwise.
  */
int SenseBoxBLE::addCharacteristic(const char* characteristicUUID){
  return port.parseResponse(String("AT+UBTGCHA=")+characteristicUUID+",1a,1,1",1000);
}

int SenseBoxBLE::setConfigCharacteristic(const char* serviceUUID, const char* characteristicUUID){
  if (h_configCharacteristic != 0) {
    return -1;
  }
  SenseBoxBLE::addService(serviceUUID);
  h_configCharacteristic = SenseBoxBLE::addCharacteristic(characteristicUUID);
  return h_configCharacteristic;
}

bool parseMyValue(uint8_t* target, String s){
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

void SenseBoxBLE::configCharacteristicWritten(){
  if(configHandler!=nullptr){
    (*configHandler)();
  }
}


/**
  * @brief Polls the BLE module for incoming data.
  *
  * @param timeout The timeout in milliseconds.
  */
void SenseBoxBLE::poll(int timeout)
{
  auto starttime=millis();
  while(millis()-starttime<timeout){
    poll();
    delay(5);
  }
}

/**
  * @brief Polls the BLE module for incoming data.
  *
  */
void SenseBoxBLE::poll(){
    memset(configCharValue, 0, sizeof(configCharValue));
    if(port.poll()){
      if(parseMyValue(configCharValue,port.checkCharWritten(h_configCharacteristic))){
            configCharacteristicWritten();
        }
        port.flushInput();
    }

}

void SenseBoxBLE::read(uint8_t *arrayPointer, unsigned int arraySize)
{
  memcpy(arrayPointer, configCharValue, arraySize);
}

void SenseBoxBLE::read(float& f)
{
  memcpy(&f,configCharValue,4);
}

/**
  * @brief Writes a float value to a characteristic.
  *
  * @param characteristic The ID of the characteristic to write to.
  * @param value The float value to write.
  * @return true if the write was successful, false otherwise.
  */
bool SenseBoxBLE::write(int characteristic, float& value)
{
  data = reinterpret_cast<uint8_t*>(&value);
  return port.writeValue(characteristic,data,4);
}

/**
  * @brief Writes two float values to a characteristic.
  *
  * @param characteristic The ID of the characteristic to write to.
  * @param f1 The first float value to write.
  * @param f2 The second float value to write.
  * @return true if the write was successful, false otherwise.
  */
bool SenseBoxBLE::write(int characteristic, float& f1, float& f2)
{
  float array[2] = {f1, f2};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,8);
}

/**
  * @brief Writes three float values to a characteristic.
  *
  * @param characteristic The ID of the characteristic to write to.
  * @param f1 The first float value to write.
  * @param f2 The second float value to write.
  * @param f3 The third float value to write.
  * @return true if the write was successful, false otherwise.
  */
bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3)
{
  float array[3] = {f1, f2, f3};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,12);
}

 /**
  * @brief Writes four float values to a characteristic.
  *
  * @param characteristic The ID of the characteristic to write to.
  * @param f1 The first float value to write.
  * @param f2 The second float value to write.
  * @param f3 The third float value to write.
  * @param f4 The fourth float value to write.
  * @return true if the write was successful, false otherwise.
  */
bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3 , float& f4)
{
  float array[4] = {f1, f2, f3, f4};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,16);
}

/**
  * @brief Writes five float values to a characteristic.
  *
  * @param characteristic The ID of the characteristic to write to.
  * @param f1 The first float value to write.
  * @param f2 The second float value to write.
  * @param f3 The third float value to write.
  * @param f4 The fourth float value to write.
  * @param f5 The fifth float value to write.
  * @return true if the write was successful, false otherwise.
  */
bool SenseBoxBLE::write(int characteristic, float& f1, float& f2, float& f3 , float& f4, float& f5)
{
  float array[5] = {f1, f2, f3, f4, f5};
  data = reinterpret_cast<uint8_t*>(array);
  return port.writeValue(characteristic,data,20);
}

//#endif
