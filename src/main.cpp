#include <Arduino.h>
#include <Wire.h>

class IVoltageSensor{
  public:
    virtual float getVoltage()=0;
    virtual ~IVoltageSensor()=default;
};

class MockSensor:public IVoltageSensor{
  private:
    float testVoltages[5]={4.1,3.8,3.1};
    int index=0;
  public:
    float getVoltage() override{
      float v=testVoltages[index];
      index=(index+1)%3;
      return v;
    }
};

enum class SystemState{
  INITIALIZING,
  MEASURING,
  TELEMETRY,
  SAFE_MODE,
  GOTO_SLEEP,
};

class PowerSystem{
  private:
    SystemState currentState;
    IVoltageSensor*sensor;
  public:
    PowerSystem(IVoltageSensor*s):currentState(SystemState::MEASURING),sensor(s){}

    void begin(){
      Serial.begin(115200);
      Wire.begin();
      Serial.println("Power System Starting");
      currentState=SystemState::MEASURING;
    }
  
    void run(){
      switch(currentState){
        case SystemState::INITIALIZING:
          break;
        case SystemState::MEASURING:
          performMeasure();
          break;
        case SystemState::TELEMETRY:
          sendData();
          break;
        case SystemState::SAFE_MODE:
          enterSafeMode();
          break;
        case SystemState::GOTO_SLEEP:
          enterDeepSleep();
          break;
      }
    }
  private:
    void performMeasure(){
      float voltage=sensor->getVoltage();
      Serial.print("voltage:");
      Serial.print(voltage);
      Serial.println(" V");

      if (voltage<3.3){
        currentState=SystemState::SAFE_MODE;
      }else{
        currentState=SystemState::TELEMETRY;
      }
    }
    
    void sendData(){
      Serial.println("Sending telemetry data");
      currentState=SystemState::MEASURING;
    }
    void enterSafeMode(){
      Serial.println("Entering Safe Mode");
      currentState=SystemState::GOTO_SLEEP;
    }
    void enterDeepSleep(){
      Serial.println("Entering Deep Sleep");
      Serial.flush();
      esp_sleep_enable_timer_wakeup(5*1000000);
      esp_deep_sleep_start();
    }
};

MockSensor dummySensor;
PowerSystem eps(&dummySensor);

void setup(){
  eps.begin();
}

void loop(){
  eps.run();
  delay(50);
}