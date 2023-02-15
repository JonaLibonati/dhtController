
// FILE: dht.h
// AUTHOR: Jonathan Libonati
// VERSION: 0.0.1
// PURPOSE: DHT Temperature & Humidity Digital Sensor controller for Arduino

#ifndef dht_h
#define dht_h

#include <Arduino.h>

#define DHT11_LIB_VER "0.0.1"

// For a 16Mhz Processor, 1 cycle = 0.0625 microsec.

#define DHT11_UNSTABLE_TIME 1000 // (miliseconds)
#define DHT11_START_SIGNAL 20 // T_be (milisec)
#define DHT11_WAIT_TIME  13 // T_go (microsec)
//#define DHT11_TEST_LOW_MIN 81 // T_rel_min (microsec)
//#define DHT11_TEST_LOW_MAX 85 // T_rel_max (microsec)
//#define DHT11_TEST_HIGH_MIN 85 // T_reh_min (microsec)
//#define DHT11_TEST_HIGH_MAX 88 // T_reh_max (microsec)
//#define DHT11_LOW_SIGNAL_MIN 52 // T_LOW_min (microsec)
//#define DHT11_LOW_SIGNAL_MAX 56 // T_LOW_max (microsec)
//#define DHT11_HIGH0_SIGNAL_MIN 23 // T_H_min (microsec)
//#define DHT11_HIGH0_SIGNAL_MAX 27 // T_H_max (microsec)
//#define DHT11_HIGH1_SIGNAL_MIN 68 // T_H_min (microsec)
//#define DHT11_HIGH1_SIGNAL_MAX 74 // T_H_max (microsec)
//#define DHT11_RELEASE_TIME 60 // Ten (microsec) (It is more than its max 56 microsec)

#define DHT11_TIMEOUT 100

#define DHT11_ERROR_SUM -1
#define DHT11_ERROR_TIMEOUT -2
#define DHT11_NON_VALID -111
#define DHT11_ALL_OK 0

class dht
{
private:
    int _listenLOW (uint8_t bit_mask, uint8_t port);
    int _listenHIGH (uint8_t bit_mask, uint8_t port);
    int _read(uint8_t pin);

public:
    int read(uint8_t pin);
    double humidity;
    double temperature;
    uint8_t byte[5];  // buffer to receive data
};

#endif