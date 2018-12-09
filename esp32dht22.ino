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

const int LED = 2;

ulong lastConnected = 0;

Neotimer timer;

const int DHTPin = 25;
DHTesp dht;

void connectBlynk()
{
    digitalWrite(LED, LOW);
    Blynk.connectWiFi(ssid, pass);
    if (WiFi.status() == WL_CONNECTED)
    {
        Blynk.config(auth);
        Blynk.connect();
    }
    lastConnected = millis();
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    connectBlynk();
    dht.setup(DHTPin, DHTesp::DHT22);
    timer.set(3000);
}

void runBlynkOrReconnectWifi()
{
    if (Blynk.connected())
    {
       Blynk.run();
       lastConnected = millis();
       digitalWrite(LED, HIGH);
    } 
    else 
        if (millis() > (lastConnected+60000)) // last connected at 1 minute ago
            connectBlynk();
}

void sendSensorData()
{
    TempAndHumidity tah = dht.getTempAndHumidity();
    Blynk.virtualWrite(V5, tah.humidity);
    Blynk.virtualWrite(V6, tah.temperature);
}

void loop()
{
    runBlynkOrReconnectWifi();
    if (timer.repeat())
        sendSensorData();
}
