#define BLYNK_TEMPLATE_ID "TMPLjjFX70-U"
#define BLYNK_DEVICE_NAME "Smart Cannabis Farm"
#define BLYNK_AUTH_TOKEN "uKjHEwo8ens6jJbqhQOYVn3-Q88LzCpS"
#define SDA_1 12
#define SCL_1 14

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Wire.h"
#include "SHT2x.h"

const int relaylight = 12;
const int relayfan = 13;
const int relaypump = 14;

char auth[] = "uKjHEwo8ens6jJbqhQOYVn3-Q88LzCpS";
char ssid[] = "Napapat_2.4G";
char pass[] = "kai949456";

SHT2x sht;


float humid = 0;
float temp = 0;
int pump;
int fan;
int light;

bool mode = 0;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth , ssid , pass);
  sht.begin(SDA_1, SCL_1);
  
  pinMode(relaylight , OUTPUT);
  pinMode(relayfan , OUTPUT);
  pinMode(relaypump , OUTPUT);

  Blynk.virtualWrite(V3 , 0);
  Blynk.virtualWrite(V4 , 0);
  Blynk.virtualWrite(V5 , HIGH);
  Blynk.virtualWrite(V6 , HIGH);
  Blynk.virtualWrite(V7 , HIGH);


}

void loop() {
  Blynk.run();
  GetTempHumi();
}

BLYNK_WRITE(V5)
{
    light = param.asInt();
    
    if( light == 0){
       Blynk.virtualWrite(V0 , 1);
    }
    else{
      Blynk.virtualWrite(V0 , 0);
    }
    digitalWrite(relaylight , light);
}

BLYNK_WRITE(V6)
{
    fan = param.asInt();
    
    if( fan == 0){
      Blynk.virtualWrite(V1 , 1);
    }
    else{
      Blynk.virtualWrite(V1 , 0);
    }
    digitalWrite(relayfan , fan);
}

BLYNK_WRITE(V7)
{
    pump = param.asInt();
    
    if( pump == 0){
      Blynk.virtualWrite(V2 , 1);
    }
    else{
      Blynk.virtualWrite(V2 , 0);
    }
    digitalWrite(relaypump , pump);
}

void GetTempHumi(){
  sht.read();
  
  if (sht.getTemperature() <= 40 && sht.getTemperature() >= 0) {
  humid = sht.getHumidity();
  temp = sht.getTemperature();
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, humid);
  Serial.print(temp);
  Serial.print("\t");
  Serial.println(humid);
  }
  Serial.println("");
  delay(100);
}


