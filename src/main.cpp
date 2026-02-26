#include <Wire.h>

enum class SystemState {
    INITIALIZING,
    MEASURING,    // Pomiar prądów/napięć przez INA3221
    TELEMETRY,    // Wysyłka danych przez Wi-Fi
    SAFE_MODE,    // Ochrona baterii 
    GOTO_SLEEP    // Przygotowanie do Deep Sleep
};

class DeskSatEPS {
private:
    SystemState currentState;
    unsigned long lastMeasureTime;
    const unsigned long measureInterval = 5000; /

public:
    DeskSatEPS() : currentState(SystemState::INITIALIZING), lastMeasureTime(0) {}

    void begin() {
        Serial.begin(115200);
        Wire.begin(); 
        currentState = SystemState::MEASURING;
    }

    void run() {
        switch (currentState) {
            case SystemState::MEASURING:
                if (millis() - lastMeasureTime >= measureInterval) {
                    performMeasurement();
                    lastMeasureTime = millis();
                }
                break;

            case SystemState::TELEMETRY:
                sendData();
                break;

            case SystemState::SAFE_MODE:
                handleSafety();
                break;

            case SystemState::GOTO_SLEEP:
                enterDeepSleep();
                break;
        }
    }

private:
    void performMeasurement() {
        float busVoltage = 3.8; 
        
        if (busVoltage < 3.2) { 
            currentState = SystemState::SAFE_MODE;
        } else {
            currentState = SystemState::TELEMETRY;
        }
    }

    void sendData() {
        currentState = SystemState::GOTO_SLEEP;
    }

    void handleSafety() {
        Serial.println("CRITICAL: Battery Low!");
        currentState = SystemState::GOTO_SLEEP;
    }

    void enterDeepSleep() {
        Serial.println("Entering Deep Sleep (10uA target)...");
        esp_sleep_enable_timer_wakeup(60 * 1000000); 
        esp_deep_sleep_start(); 
    }
};

DeskSatEPS eps;

void setup() { eps.begin(); }
void loop() { eps.run(); } 