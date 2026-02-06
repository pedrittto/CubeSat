#include <Arduino.h>

const int PIN_BATTERY_SENSOR=34;
const float MAX_VOLTAGE=3.3;
const int ADC_RESOLUTION=4095;
const float LOW_BATTERY_THRESHOLD=3.0;
float currentVoltage=0.0;

float readBatteryVoltage(){
  int rawValue=analogRead(PIN_BATTERY_SENSOR);
  float voltage= (float(rawValue)/ADC_RESOLUTION)*MAX_VOLTAGE;
  return voltage;
}

void  checkBatteryStatus(){
  currentVoltage=readBatteryVoltage();

  Serial.print("BATTERY VOLTAGE: ");
  Serial.print(currentVoltage);
  Serial.println(" V");

  if (currentVoltage<LOW_BATTERY_THRESHOLD){
    Serial.println("LOW BATTERY!");
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(PIN_BATTERY_SENSOR,INPUT);
}

void loop(){
  checkBatteryStatus();
  delay(1000);
}