# Solar BMS / EPS Prototype

## About the Project
This project is a prototype of an Electrical Power System (EPS) and Battery Management System (BMS) powered by solar panels. The hardware and software are designed with CubeSat platforms in mind. 

## Hardware Architecture
The system relies on proven components for power optimization and monitoring:
* **ESP32** - The main microcontroller handling the system logic.
* **INA3221** - A 3-channel voltage and current monitor (reading solar and battery shunts via a custom analog RC low-pass filter).
* **TPS63001 (Texas Instruments)** - A Buck-Boost converter providing a stable 3.3V power supply.
* **Solar cells & Battery** - The primary energy source and buffer.

## Software Architecture
* **Object-Oriented Programming:** The code is modularized into classes utilizing a Hardware Abstraction Layer.
* **Dependency Injection & Mocking:** The business logic (state machine) can be tested entirely without physical sensors by injecting virtual interfaces (MockSensors).
* **State Machine:** A non-blocking execution flow managing the device's lifecycle (`MEASURING`, `TELEMETRY`, `SAFE_MODE`).
* **Power Optimization:** Implementation of a hardware Deep Sleep mode, targeting a current draw of ~10 µA.
