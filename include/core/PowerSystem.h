#pragma once
#include "comms/Telemetry.h"
#include "hal/IVoltageSensor.h"

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
        IVoltageSensor* sensor;
        float lastVoltage;
    
    public:
        PowerSystem(IVoltageSensor*s);
        void begin();
        void run();
    
    private:
        void performMeasure();
        void sendData();
        void enterSafeMode();
        void enterDeepSleep();
};