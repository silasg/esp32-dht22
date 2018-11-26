#define BLYNK_PRINT Serial // Comment this out to disable prints and save space 

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"
#include <neotimer.h>
#include "secrets.h"

char auth[] = BLYNK_AUTH_TOKEN; // defined in .gitignore'd secrets.h

char ssid[] = WIFI_SSID; // defined in .gitignore'd secrets.h
char pass[] = WIFI_PASSWORD; // Set password to "" for open networks. defined in .gitignore'd secrets.h 

Neotimer timer;

const int DHTPin = 25;
DHTesp dht;

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    dht.setup(DHTPin, DHTesp::DHT22);
    timer.set(3000);
}

void sendSensorData() {
    TempAndHumidity tah = dht.getTempAndHumidity();
    Blynk.virtualWrite(V5, tah.humidity);
    Blynk.virtualWrite(V6, tah.temperature);
}

void loop()
{
    Blynk.run();
    if (timer.repeat())
        sendSensorData();
}
