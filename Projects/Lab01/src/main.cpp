#define BLYNK_PRINT Serial    
#include <WiFi.h>
#include <Wire.h>
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

void writeI2CRegister8bit(int addr, int value) 
{
  Wire.beginTransmission(addr); 
  Wire.write(value); 
  Wire.endTransmission(); 
} 

unsigned int readI2CRegister16bit(int addr, int reg) 
{ 
  Wire.beginTransmission(addr); 
  Wire.write(reg); Wire.endTransmission(); 
  delay(1);  Wire.requestFrom(addr, 2); 
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read(); return t; 
}

unsigned int readCapacitance() 
{ 
  return readI2CRegister16bit(0x20, 0); 
} 

void sendSensor()
{
  int light = analogRead(LDR_PIN);
  Blynk.virtualWrite(V5,light);
  float h = dht.readHumidity();     
  Blynk.virtualWrite(V6, h); 
  float t = dht.readTemperature();     
  Blynk.virtualWrite(V7, t);
  int cap = readCapacitance();
  Blynk.virtualWrite(V8, cap);
}

void setup() 
{
  Wire.begin();  
  writeI2CRegister8bit(0x20, 6);
  delay(1);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L,sendSensor);
}
void loop() 
{
  Blynk.run();
  timer.run();
}

