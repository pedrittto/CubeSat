#pragma once

class IVoltageSensor{
  public:
    virtual float getVoltage()=0;
    virtual ~IVoltageSensor()=default;
};

