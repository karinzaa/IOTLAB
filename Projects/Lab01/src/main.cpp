#define BLYNK_PRINT Serial    
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>    
char auth[] = "";
char ssid[] = "";
char pass[] = "";
#define LDR_PIN 33 
#define DHTPIN 19 
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;
void sendSensor()
{
  int light = analogRead(LDR_PIN);
  Blynk.virtualWrite(V5,light);

  float h = dht.readHumidity();    
  Blynk.virtualWrite(V6, h); 
  float t = dht.readTemperature();     
  Blynk.virtualWrite(V7, t);
}

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L,sendSensor);
}
void loop() 
{
  Blynk.run();
  timer.run();
}

