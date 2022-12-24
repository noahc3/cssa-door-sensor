#include <Arduino.h>
#include <EEPROM.h>

#include "configmgr.hpp"
#include "constants/pins.hpp"
#include "ledutils.hpp"
#include "setupmgr.hpp"
#include "wifimgr.hpp"
#include "doorstatemgr.hpp"

#define TICK_DELAY 10

LEDUtils LED;
WifiManager WifiMgr;
ConfigManager ConfigMgr;
SetupManager SetupMgr;
DoorStateManager DoorStateMgr;

void startBackgroundThread();
void asyncTick(void *parameter);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing!");

    LED.setBrightness(0.3);
    LED.set(255, 0, 0);

    startBackgroundThread();

    SetupMgr.start();

    WifiMgr.connect();
}

void loop() {
    Serial.printf("Loop - %.2fs\n", (millis() / 1000.0));

    bool doorState = DoorStateMgr.getState();
    String doorStateString = DoorStateMgr.getStateString();
    Serial.println(doorStateString);

    if (doorState) {
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
        DoorStateMgr.tick();
        delay(TICK_DELAY);
    }
}