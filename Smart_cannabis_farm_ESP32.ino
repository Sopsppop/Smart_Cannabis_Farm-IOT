#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLjjFX70-U"
#define BLYNK_DEVICE_NAME "Smart Cannabis Farm"
#define BLYNK_AUTH_TOKEN "uKjHEwo8ens6jJbqhQOYVn3-Q88LzCpS"
#define SDA_1 2
#define SCL_1 15

#include "Wire.h"
#include "SHT2x.h"
#include "EasyNextionLibrary.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const int relaylight = 21;
const int relayfan = 19;
const int relaypump = 18;

char auth[] = "uKjHEwo8ens6jJbqhQOYVn3-Q88LzCpS";
char ssid[] = "Napapat_2.4G";
char pass[] = "kai949456";

SHT2x sht;

EasyNex myNex(Serial2);

uint32_t start;
uint32_t stop;

int pump;
int fan;
int light;

float humid = 0;
float temp = 0;
int sethumid = 0;
int settemp = 0;

bool mode;

void setup() {

    Serial.begin(115200);
    myNex.begin(9600);
    Blynk.begin(auth, ssid, pass);
    sht.begin(SDA_1, SCL_1);

    pinMode(relaylight, OUTPUT);
    pinMode(relayfan, OUTPUT);
    pinMode(relaypump, OUTPUT);

    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V5, 1);
    Blynk.virtualWrite(V6, 1);
    Blynk.virtualWrite(V7, 1);

    digitalWrite(relaylight, 1);
    digitalWrite(relaypump, 1);
    digitalWrite(relayfan, 1);

}

void loop() {
    Blynk.run();
    GetTempHumi();
    myNex.NextionListen();
    switch (mode){
        case 0: {
            Blynk.setProperty(V5, "isDisabled", false);
            Blynk.setProperty(V6, "isDisabled", false);
            Blynk.setProperty(V7, "isDisabled", false);
            break;
        }
        case 1: {

            Blynk.setProperty(V5, "isDisabled", true);
            Blynk.setProperty(V6, "isDisabled", true);
            Blynk.setProperty(V7, "isDisabled", true);
            automation(0);
            break;
        }
    } 
}

BLYNK_WRITE(V5) {
    light = param.asInt();

    if (light == 0) {
        Blynk.virtualWrite(V0, 1);
    } else {
        Blynk.virtualWrite(V0, 0);
    }
    Conlight(light);
}

BLYNK_WRITE(V6) {

    fan = param.asInt();

    if (fan == 0) {
        Blynk.virtualWrite(V1, 1);
    } else {
        Blynk.virtualWrite(V1, 0);
    }
    Confan(fan);

}

BLYNK_WRITE(V7) {

    pump = param.asInt();

    if (pump == 0) {
        Blynk.virtualWrite(V2, 1);
    } else {
        Blynk.virtualWrite(V2, 0);
    }
    Conpump(pump);

}

BLYNK_WRITE(V40) {

    mode = param.asInt();

}

BLYNK_WRITE(V8) {

    settemp = param.asInt();

}

BLYNK_WRITE(V9) {

    sethumid = param.asInt();

}

void GetTempHumi() {

    start = micros();
    sht.read();
    stop = micros();

    if (sht.getTemperature() <= 50 && sht.getTemperature() >= 0) {
        humid = sht.getHumidity();
        temp = sht.getTemperature();
        myNex.writeStr("t0.txt" , String(temp, 1));
        myNex.writeStr("t1.txt" , String(humid, 1));
        Blynk.virtualWrite(V3, temp);
        Blynk.virtualWrite(V4, humid);
        Serial.print(temp);
        Serial.print(":");
        Serial.print("\t");
        Serial.println(humid);
        Serial.print(":");
    }
    Serial.println("");
    delay(100);

}

void Conlight(bool status) {
    digitalWrite(relaylight, status);
    Serial.print(status);
    Blynk.virtualWrite(V5, status);
}

void Confan(bool status) {
    digitalWrite(relayfan, status);
    Serial.print(status);
    Blynk.virtualWrite(V6, status);
}

void Conpump(bool status) {
    digitalWrite(relaypump, status);
    Serial.print(status);
    Blynk.virtualWrite(V7, status);
}

void automation(bool status){
    return;
}


