#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "" //Digite o nome da sua rede entre as aspas
#define WIFI_PASS "" //Digite a sua senha de rede entre as aspas

fauxmoESP fauxmo;

#define SERIAL_BAUDRATE     115200

#define LED_WHITE           2
#define ID_WHITE            "abajur de cama"
#define ID_WHITE2           "abajur matinal" 

void wifiSetup() {

    // define wifi pro modo STA
    WiFi.mode(WIFI_STA);

    // Conecta
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // espera
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

const byte Bright = 255;

int brightness = 0;    
int fadeAmount = 1;

void aumentaLuz(unsigned char brightGoal){
  for(brightness; brightness < brightGoal; brightness+=fadeAmount){
      analogWrite(LED_WHITE, brightness);
      delay(30);
  }
}

void diminuiLuz(unsigned char brightGoal){
  for(brightness; brightness > brightGoal; brightness+=fadeAmount){
      analogWrite(LED_WHITE, brightness);
      delay(30);
  }
}

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    pinMode(LED_WHITE, OUTPUT);
    digitalWrite(LED_WHITE, LOW);

    wifiSetup();

    fauxmo.createServer(true);
    fauxmo.setPort(80);
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice(ID_WHITE);
    fauxmo.addDevice(ID_WHITE2);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
        if (strcmp(device_name, ID_WHITE)==0) {
          if(state){ // acende em 2s pro nível de luz desejado (value)
            if((int)value > brightness){
              fadeAmount = 1;
              aumentaLuz(value);
            }else{
              fadeAmount = -1;
              diminuiLuz(value);
            }
          }else{// apaga
            fadeAmount = -1;
            diminuiLuz(0);
            digitalWrite(LED_WHITE, LOW);
          }
          
        }else 
        if (strcmp(device_name, ID_WHITE2)==0) {
          if(state){//acende em meia hora
            for(brightness; brightness < 255; brightness++){
              analogWrite(LED_WHITE, brightness);
              delay(7060); // a cada 7 segundos o valor do abajur sobe em 1
            }
          }else{// apaga
            fadeAmount = -1;
            diminuiLuz(0);
            digitalWrite(LED_WHITE, LOW);
          }
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
