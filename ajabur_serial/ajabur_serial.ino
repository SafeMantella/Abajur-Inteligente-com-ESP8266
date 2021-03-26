#include <SoftwareSerial.h>

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

#define WIFI_SSID "LAR DOCE LAR 2G"
#define WIFI_PASS "m26022004"

fauxmoESP fauxmo;

#define SERIAL_BAUDRATE     115200
#define LED_YELLOW          2
#define ID_YELLOW           "abajur"

void wifiSetup() {
    WiFi.mode(WIFI_STA);

    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    pinMode(LED_YELLOW, OUTPUT);
    digitalWrite(LED_YELLOW, LOW);
    
    wifiSetup();

    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    fauxmo.enable(true);
    fauxmo.addDevice(ID_YELLOW); 
    
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value, unsigned int hue, unsigned int saturation, unsigned int ct) {
        Serial.printf("state: %s value: %d hue: %d saturation: %u ct: %u\n", state ? "ON" : "OFF", value, hue, saturation, ct);
        
        if (strcmp(device_name, ID_YELLOW)==0) {
            digitalWrite(LED_YELLOW, state ? HIGH : LOW);
        }
    });
}

void loop() {
    fauxmo.handle();

    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }
}
