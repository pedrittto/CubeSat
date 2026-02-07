#include <Arduino.h>

const int PIN_BATTERY_SENSOR=34;
const float MAX_VOLTAGE=3.3;
const float MIN_VOLTAGE=0.0;
const int ADC_RESOLUTION=4095;
const float LOW_BATTERY_THRESHOLD=1.0;
const int SAMPLES_COUNT=10;

float currentVoltage=0.0;

float readBatteryVoltage(){
  long rawSum=0;

  for(int i=0;i<SAMPLES_COUNT;i++){
    rawSum+=analogRead(PIN_BATTERY_SENSOR);
    delay(2);
  }
  float rawAverage=rawSum/(float)SAMPLES_COUNT;
  float voltage=(rawAverage/ADC_RESOLUTION)*MAX_VOLTAGE;
  return voltage;
}

float readBatteryStatus(){
  int rawValue=analogRead(PIN_BATTERY_SENSOR);
  float voltage=(float(rawValue)/ADC_RESOLUTION)*MAX_VOLTAGE;

  return voltage;
}

int calculatePercentage(float voltage){
  float percentage=(voltage-MIN_VOLTAGE)/(MAX_VOLTAGE-MIN_VOLTAGE)*100;
  return(int)percentage;
}

void checkBatteryStatus(){
  currentVoltage=readBatteryVoltage();

  int batteryLevel=calculatePercentage(currentVoltage);

  Serial.print("BATTERY: ");
  Serial.print(currentVoltage);
  Serial.print(" V [");
  Serial.print(batteryLevel);
  Serial.println("%]");
  
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
  delay(500);
}