#include "doorstatesvc.hpp"

#include <Arduino.h>
#include <EasyUltrasonic.h>

#include "constants/pins.hpp"

#define AUTO 0
#define CLOSED 1
#define OPEN 2

#define SWITCH_DEBOUNCE_POINTS 10
#define SWITCH_DEBOUNCE_MIN 8

#define SENSOR_DEBOUNCE_POINTS 200
#define SENSOR_DEBOUNCE_MIN 180
#define SENSOR_MAX_DISTANCE 70

EasyUltrasonic ultrasonic;

byte switchData[SWITCH_DEBOUNCE_POINTS];
byte switchDataIndex = 0;
byte switchState = false;

bool sensorData[SENSOR_DEBOUNCE_POINTS];
byte sensorDataIndex = 0;
bool sensorState = false;


DoorStateService::DoorStateService() {
    pinMode(Pins::SPDT_A, INPUT);
    pinMode(Pins::SPDT_B, INPUT);
    pinMode(Pins::SPDT_POLE, OUTPUT);

    digitalWrite(Pins::SPDT_POLE, HIGH);

    ultrasonic.attach(Pins::TRIG, Pins::ECHO);
}

bool DoorStateService::getState() {
    if (switchState == AUTO) {
        return sensorState;
    } else {
        return switchState == OPEN;
    }
}

void DoorStateService::tick() {
    tickSwitch();
    tickSensor();
}

void DoorStateService::tickSwitch() {
    byte state = querySwitchMode();
    switchData[switchDataIndex] = state;
    switchDataIndex = (switchDataIndex + 1) % SWITCH_DEBOUNCE_POINTS;

    byte closedCount = 0;
    byte openCount = 0;
    byte autoCount = 0;
    for (byte i = 0; i < SWITCH_DEBOUNCE_POINTS; i++) {
        if (switchData[i] == AUTO) {
            autoCount++;
        } else if (switchData[i] == CLOSED) {
            closedCount++;
        } else {
            openCount++;
        }
    }

    if (autoCount >= SWITCH_DEBOUNCE_MIN) {
        switchState = AUTO;
    } else if (closedCount >= SWITCH_DEBOUNCE_MIN) {
        switchState = CLOSED;
    } else if (openCount >= SWITCH_DEBOUNCE_MIN) {
        switchState = OPEN;
    }
}

void DoorStateService::tickSensor() {
    bool state = querySensorState();
    sensorData[sensorDataIndex] = state;
    sensorDataIndex = (sensorDataIndex + 1) % SENSOR_DEBOUNCE_POINTS;

    byte closedCount = 0;
    byte openCount = 0;
    for (byte i = 0; i < SENSOR_DEBOUNCE_POINTS; i++) {
        if (sensorData[i]) {
            openCount++;
        } else {
            closedCount++;
        }
    }

    if (openCount >= SENSOR_DEBOUNCE_MIN) {
        sensorState = true;
    } else if (closedCount >= SENSOR_DEBOUNCE_MIN) {
        sensorState = false;
    }
}

String DoorStateService::getStateString() {
    if (switchState == AUTO) {
        if (sensorState) {
            return "AUTO OPEN";
        } else {
            return "AUTO CLOSED";
        }
    } else if (switchState == CLOSED) {
        return "MANUAL CLOSED";
    } else if (switchState == OPEN) {
        return "MANUAL OPEN";
    }

    return "ERROR";
}

byte DoorStateService::querySwitchMode() {
    byte a = digitalRead(Pins::SPDT_A);
    byte b = digitalRead(Pins::SPDT_B);

    if (a == LOW && b == HIGH) {
        return AUTO;
    } else if (a == LOW && b == LOW) {
        return CLOSED;
    } else {
        return OPEN;
    }
}

bool DoorStateService::querySensorState() {
    float distCm = ultrasonic.getDistanceCM();
    return distCm < SENSOR_MAX_DISTANCE;
}