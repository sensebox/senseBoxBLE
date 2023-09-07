#include "NINAB31serial.h"

class SenseBoxBLE
{
    private:

        static uint8_t data_package[20];

        static NINAB31Serial port;

        static uint8_t* data; //this pointer points to the data the user wants to write in the characteristic


    public:

        static void start(const char* DEVICE_NAME, uint8_t* p, size_t n = 0);
        static void start(const char* DEVICE_NAME);
        static void start(uint8_t* p, size_t n = 0);
        static void start();

        static bool advertise();
        static bool stopAdvertise();

        static int addService(const char* serviceUUID);
        static int addCharacteristic(const char* characteristicUUID, int properties = 1, int permissions = 1, int maxLen = 20);

        static bool write(int, float&);
        static bool write(int, float&, float&);
        static bool write(int, float&, float&, float&);
        static bool write(int, float&, float&, float&, float&);
        static bool write(int, float&, float&, float&, float&, float&);

        static void poll();
        static void poll(int timeout);

        static void(*configHandler)();
        static uint16_t minConInterval;
        static uint16_t maxConInterval;
        static uint16_t slaveLatency;
        static uint16_t timeout;

};
