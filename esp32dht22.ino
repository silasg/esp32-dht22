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

void resetWifi()
{
    //DO NOT TOUCH
    //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
    // Serial.print("WIFI status = ");
    // Serial.println(WiFi.getMode());
    // WiFi.disconnect(true);
    // delay(1000);
    // WiFi.mode(WIFI_STA);
    // delay(1000);
    // Serial.print("WIFI status = ");
    // Serial.println(WiFi.getMode());
    // End silly stuff !!!


    WiFi.begin(ssid, pass);
    delay(1000);
    WiFi.disconnect();
    delay(1000);
    WiFi.begin(ssid, pass);
    delay(1000);
}

void connectBlynk()
{
    resetWifi();
    setLastConnectedNow();
    //Blynk.connectWiFi(ssid, pass);
    Blynk.config(auth);
    Blynk.connect();
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    connectBlynk();
    dht.setup(DHTPin, DHTesp::DHT22);
    timer.set(3000);
}

void runBlynkOrRestart()
{
    if (Blynk.connected())
    {
       Blynk.run();
       setLastConnectedNow();
       digitalWrite(LED, HIGH);
    } 
    else {
        digitalWrite(LED, LOW);
        if (lastConnectedMoreThanOneMinuteAgo()) {
            Serial.println("Rebooting ...");
            ESP.restart(); // reboot, since reconnect doesn't work sometimes...
        }
    }
}

void setLastConnectedNow() {
    lastConnected = millis();
}

bool lastConnectedMoreThanOneMinuteAgo() {
    return millis() > (lastConnected+60000);
}

void sendSensorData()
{
    TempAndHumidity tah = dht.getTempAndHumidity();
    Blynk.virtualWrite(V5, tah.humidity);
    Blynk.virtualWrite(V6, tah.temperature);
}

void loop()
{
    runBlynkOrRestart();
    if (timer.repeat())
        sendSensorData();
}
