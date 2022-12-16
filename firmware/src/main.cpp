#include <Arduino.h>
#include <EEPROM.h>
#include <EasyUltrasonic.h>

#include "configmgr.hpp"
#include "constants/pins.hpp"
#include "ledutils.hpp"
#include "setupmgr.hpp"
#include "wifimgr.hpp"

#define TICK_DELAY 10

EasyUltrasonic ultrasonic;
LEDUtils LED;
WifiManager WifiMgr;
ConfigManager ConfigMgr;
SetupManager SetupMgr;

void startBackgroundThread();
void asyncTick(void *parameter);

void setup() {
    LED.setBrightness(0.3);
    LED.set(255, 0, 0);

    Serial.begin(115200);
    Serial.println("Initializing!");

    startBackgroundThread();

    SetupMgr.start();

    WifiMgr.connect();

    ultrasonic.attach(Pins::TRIG, Pins::ECHO);
}

void loop() {
    Serial.println("Loop!");
    float distCm = ultrasonic.getDistanceCM();

    Serial.print(distCm);
    Serial.println(" cm");

    if (distCm < 15) {
        LED.set(0, 255, 0);
    } else {
        LED.set(0, 0, 255);
    }

    delay(1000);
}

void startBackgroundThread() {
    Serial.println("Starting background thread");
    xTaskCreatePinnedToCore(asyncTick, "Background Thread", 10000, NULL, 1,
                            NULL, 0);
}

void asyncTick(void *parameter) {
    for (;;) {
        LED.tick();
        delay(TICK_DELAY);
    }
}