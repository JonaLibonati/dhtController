# dth.h
The C++ class allows to control a DTH digital temperature and humidity sensor on ARDUINO.

Tested on Arduino Uno (16 Mhz).

Tested on DTH 11.

The code is made based on [the DTH manual](https://www.maritex.com.pl/product/attachment/147097/e3ba4b862ce252dac75137c2b42b983f).

## Implementation
Download the files and compress in a .zip file.

In the Arduino IDE, navigate to Sketch > Include Library > Add . ZIP Library and add the .zip.

## Class dht
This class creates a dth object.

dht.read(uint8_t pin) method should be used to update the temperature and humidity values. The method requests the data to the sensor by using a single bus communication.

[For further information about the DTH sensor, click here](https://www.maritex.com.pl/product/attachment/147097/e3ba4b862ce252dac75137c2b42b983f).

It is recommended:

* to use the 4.7K pull-up resistor when the cable length is shorter than 5m. (Also tested with 10k resistor)

* to read the sensor with interval of at least 2 seconds.

* The temperature and humidity data read from DHT11 is always the previous measurement. If the interval time between the two reads is very long, it is recommended to use dht.read(uint8_t pin) two times in order to discard the outdated values.

## Example

Here is an example of reding the DHT every 3 seconds.

```
#include <dht.h>

dht DHT;

#define DHT11_PIN 2

void setup(){
  Serial.begin(9600);
}

void loop(){
  int response = DHT.read(DHT11_PIN);
  Serial.print("Code response: ");
  Serial.print(response);
  Serial.println(" ");
  Serial.print("Bytes [Binary]: ");
  Serial.print(DHT.byte[0], BIN);
  Serial.print(" ");
  Serial.print(DHT.byte[1], BIN);
  Serial.print(" ");
  Serial.print(DHT.byte[2], BIN);
  Serial.print(" ");
  Serial.print(DHT.byte[3], BIN);
  Serial.print(" ");
  Serial.print(DHT.byte[4], BIN);
  Serial.println(" ");
  Serial.print("Bytes: ");
  Serial.print(DHT.byte[0]);
  Serial.print(" ");
  Serial.print(DHT.byte[1]);
  Serial.print(" ");
  Serial.print(DHT.byte[2]);
  Serial.print(" ");
  Serial.print(DHT.byte[3]);
  Serial.print(" ");
  Serial.print(DHT.byte[4]);
  Serial.println("");
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.print(" °C");
  Serial.println("");
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity);
  Serial.print(" %HR");
  Serial.println("");
  Serial.println("");
  delay(3000);
}
```

Result (one read):

```
Bytes [Binary]: 11101 111 11011 1001 1001000
Bytes: 29 7 27 9 72
Temperature: 27.90 °C
Humidity: 29.70 %HR
```