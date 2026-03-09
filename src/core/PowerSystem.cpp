#include "core/PowerSystem.h"
#include "comms/Telemetry.h"
#include <Arduino.h>


PowerSystem::PowerSystem(IVoltageSensor*s):
    currentState(SystemState::INITIALIZING),sensor(s),lastVoltage(0.0f){}

void PowerSystem::begin(){
    Serial.begin(115200);
    delay(100);
    Serial.println("Power System Starting");
    currentState=SystemState::MEASURING;
}

void PowerSystem::run(){
    switch (currentState){
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

void PowerSystem::performMeasure(){
    float lastVoltage = sensor -> getVoltage();
    if (lastVoltage < 3.3f){
        currentState= SystemState::SAFE_MODE;
    }else{
        currentState= SystemState::TELEMETRY;
    }
}

void PowerSystem::sendData(){
    EpsTelemetry packet;

    packet.sync_byte = 0xAA;
    packet.v_batt_mv = static_cast<uint16_t>(lastVoltage*1000);
    packet.system_state = static_cast<uint8_t>(currentState);
    packet.i_solar_ma = 0; //Placeholder for solar measure
    
    uint8_t* raw = reinterpret_cast<uint8_t*>(&packet);
    packet.checkSum = 0;
    
    for (size_t i  = 0; i < sizeof(EpsTelemetry) - 1; i++){
        packet.checkSum ^= raw [i];
    }

    Serial.write(raw, sizeof(EpsTelemetry));
    currentState = SystemState::GOTO_SLEEP;
}

void PowerSystem::enterSafeMode(){
    Serial.println("Entering Safe Mode");
    currentState = SystemState::GOTO_SLEEP;
}

void PowerSystem::enterDeepSleep(){
    Serial.println("Entering Deep Sleep");
    Serial.flush();  
    
    esp_sleep_enable_timer_wakeup(5*1000000);
    esp_deep_sleep_start();
}      