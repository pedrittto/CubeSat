#include <Arduino.h>

const int PIN_BATTERY_SENSOR = 34;
const float MAX_VOLTAGE = 3.3;
const float LOW_BATTERY_VOLTAGE = 3.0;
const float BAT_MIN_VOLTAGE = 3.0;
const float BAT_MAX_VOLTAGE = 3.3;
const int ADC_RESOLUTION = 4095;
const int SAMPLES_COUNT = 10;
const int TIME_TO_SLEEP = 10;

float currentVoltage = 0.0;

float readBatteryVoltage() {
  long rawSum = 0;
  for (int i = 0; i < SAMPLES_COUNT; i++) {
    rawSum += analogRead(PIN_BATTERY_SENSOR);
    delay(2);
  }
  float rawAvg = rawSum / float(SAMPLES_COUNT);
  return (rawAvg / ADC_RESOLUTION) * MAX_VOLTAGE;
}

int calculatePercentage(float voltage) {
  float percentage = (voltage - BAT_MIN_VOLTAGE) / (BAT_MAX_VOLTAGE - BAT_MIN_VOLTAGE) * 100;
  if (percentage > 100) percentage = 100;
  if (percentage < 0) percentage = 0;
  return (int)percentage;
}

void checkBatteryStatus() {
  currentVoltage = readBatteryVoltage();
  int batteryLevel = calculatePercentage(currentVoltage);

  Serial.print("BATT: ");
  Serial.print(currentVoltage);
  Serial.print(" V [");
  Serial.print(batteryLevel);
  Serial.println(" %]");

  if (currentVoltage <= LOW_BATTERY_VOLTAGE) {
    Serial.println(">>> CRITICAL BATTERY LEVEL! <<<");
    Serial.println("System cutoff. Simulating Deep Sleep (10s)...");
    
    delay(TIME_TO_SLEEP * 1000); 
    
    Serial.println("\n--- Waking up. Resuming monitoring ---");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BATTERY_SENSOR, INPUT);
  Serial.println("\n--- BMS started. Monitoring active ---");
}

void loop() {
  checkBatteryStatus();
  delay(500);
}