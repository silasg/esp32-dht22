# esp32-dht22

Read humidity and temperature from a DHT22 sensor connected to an ESP32 microcontroller and upload values to Blynk.cc cloud.

## How to start

### Prepare Hardware

Connect your DHT22 (or other DHT sensor) to GND, 5V and data to pin 25.
If you don't use a breakout board (3 pin version) but the 4 pin version of the sensor, don't forget the resistor, which you will want to use as a pullup from the data pin to VCC.

### Prepare Project in Blynk

1. Download Blynk App, register and create a project to receive an Auth Key.
2. In your project add a Value Display Widget called "temperature" connected to Virtual Pin 5, set Frequency to Push
3. In your project add a Value Display Widget called "humidity" connected to Virtual Pin 6, set Frequency to Push

### Compile Source

In Arduino Library Manager install those libs:

1. Neotimer
2. DHT_sensor_library_for_ESPx
3. Blynk

Create file secrets.h with the following content
```#define WIFI_SSID "yourssid"
#define WIFI_PASSWORD "secret123"
#define BLYNK_AUTH_TOKEN "received by mail"```

Upload sketch and open the project in your Blynk App.
