// FILE: dht.cpp
// AUTHOR: Jonathan Libonati
// VERSION: 0.0.1
// PURPOSE: DHT Temperature & Humidity Digital Sensor controller for Arduino

#include "dht.h"

int dht::read(uint8_t pin) {
    int8_t response = _read(pin);

    switch (response)
    {
    case DHT11_ALL_OK:
        // Save humidity
        humidity = byte[0] + byte[1] * 0.1;
        // Save temperature
        //The highest bit of the lower byte "byte[3]" of the temperature data is 1 when the temperature is below 0°C.
        temperature = byte[2] + (byte[3] & 127) * 0.1;
        if (byte[3] & 128)
        {
            temperature = -temperature;
        }
        return DHT11_ALL_OK;

    case DHT11_ERROR_SUM:
        humidity = DHT11_NON_VALID;
        temperature = DHT11_NON_VALID;
        return DHT11_ERROR_SUM;

    case DHT11_ERROR_TIMEOUT:
        humidity = DHT11_NON_VALID;
        temperature = DHT11_NON_VALID;
        return DHT11_ERROR_TIMEOUT;

    default:
        humidity = DHT11_NON_VALID;
        temperature = DHT11_NON_VALID;
        break;
    }
}

int dht::_read(uint8_t pin)
{
    // direct port read is faster
    uint8_t bit_mask = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);

    // TIME BUFER
    uint8_t time_low;
    uint8_t time_high;

    // INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128; // 10000000
    uint8_t val = 0;    // 00000000

    // EMPTY BUFFER
    // Data format:
    // 8bit Humidity integer data + 8bit Humidity fraction data + 8bit Temperature integer data + 8bitTemperature fraction data + 8bit Check
    for (uint8_t i = 0; i < 5; i++)
        byte[i] = 0; // 00000000 00000000 00000000 00000000 00000000

    // DHT11 should wait 1 Sec to overturn the unstable state during which can not send any instructions
    while (millis() <= DHT11_UNSTABLE_TIME)
    {
        delay(100);
    }

    // "Makeing Arduino to ask for values". The microprocessor's I / O is set to output at the same time output low, and the low hold time should not be less than 18ms or more than 30ms.
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(DHT11_START_SIGNAL);
    digitalWrite(pin, HIGH);
    delayMicroseconds(DHT11_WAIT_TIME);

    // "Making Arduino to listen the incoming values". The microprocessor's I / O is set to input mode
    pinMode(pin, INPUT);
    time_high = _listenHIGH(bit_mask, port);
    if (time_high >= DHT11_TIMEOUT) return DHT11_ERROR_TIMEOUT;

    // Now DATA pin of DHT11 is in the output state, sending the output of 83 microseconds LOW as a response signal, followed by the output HIGH 87 microseconds to inform the peripherals DHT11 is ready to send data.

    // LOW response signal
    time_low = _listenLOW(bit_mask, port);
    if (time_low >= DHT11_TIMEOUT)  return DHT11_ERROR_TIMEOUT;
    // HIGH response signal
    time_high = _listenHIGH(bit_mask, port);
    if (time_high >= DHT11_TIMEOUT) return DHT11_ERROR_TIMEOUT;

    // DHT11 start to send the 40-bit data and the microprocessor receives 40 bits of data according to the I / O level.
    // bit 0 = 54 microsec LOW level and 23-27 microsecond HIGH.
    // bit 1 = 54 microsec LOW level and 68-74 microsecond HIGH.

    for (uint8_t i = 40; i != 0; i--)
    {
        time_low = _listenLOW(bit_mask, port);
        //if (time_low >= DHT11_TIMEOUT) return DHT11_ERROR_TIMEOUT;

        time_high = _listenHIGH(bit_mask, port);
        //if (time_high >= DHT11_TIMEOUT) return DHT11_ERROR_TIMEOUT;

        // Save '1' value. Otherwise, it stays as '0'
        if ((time_high - time_low) > 0)
        {
            byte[val] |= mask;
        }
        mask >>= 1; // 10000000 > 01000000 > 00100000 > 00010000 > 00001000 > 00000100 > 00000010 > 00000001 > 00000000

        // When mask is 00000000, the byte is complete and start the next one.
        if (mask == 0)
        {
            mask = 128;
            val++;
        }
    }

    // All data is send. The microprocessor's I / O is set to output at the same time output HIGH.

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    // CHECK SUM
    // The last 8bit Check should be equal to the sum of the previous 4 bytes.
    uint8_t sum = byte[0] + byte[1] + byte[2] + byte[3];

    if (sum == byte[4]) return DHT11_ALL_OK;

    return DHT11_ERROR_SUM;
}

// Read 1 - 0 funtions and save signal time
int dht::_listenLOW(uint8_t bit_mask, uint8_t port)
{
    uint8_t microsec = 0;
    while (!(*portInputRegister(port) & bit_mask))
    {
        delayMicroseconds(1);
        ++microsec;
        if (microsec > DHT11_TIMEOUT) break;
    }
    return microsec;
}

int dht::_listenHIGH(uint8_t bit_mask, uint8_t port)
{
    uint8_t microsec = 0;
    while (*portInputRegister(port) & bit_mask)
    {
        delayMicroseconds(1);
        ++microsec;
        if (microsec > DHT11_TIMEOUT) break;
    }
    return microsec;
}
