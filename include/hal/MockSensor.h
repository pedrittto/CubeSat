#pragma once
#include "IVoltageSensor.h"


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