#include <Arduino.h>
#include <EEPROM.h>

#include "configsvc.hpp"
#include "constants/pins.hpp"
#include "ledsvc.hpp"
#include "setupsvc.hpp"
#include "wifisvc.hpp"
#include "doorstatesvc.hpp"

#define TICK_DELAY 10

LEDService LEDSvc;
WifiService WifiSvc;
ConfigManager ConfigSvc;
SetupService SetupSvc;
DoorStateService DoorStateSvc;

void startBackgroundThread();
void asyncTick(void *parameter);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing!");

    LEDSvc.setBrightness(0.3);
    LEDSvc.set(COLOR_RED);

    startBackgroundThread();

    SetupSvc.start();

    WifiSvc.connect();
}

void loop() {
    Serial.printf("Loop - %.2fs\n", (millis() / 1000.0));

    bool doorState = DoorStateSvc.getState();
    String doorStateString = DoorStateSvc.getStateString();
    Serial.println(doorStateString);

    if (doorState) {
        LEDSvc.set(COLOR_GREEN);
    } else {
        LEDSvc.set(COLOR_BLUE);
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
        LEDSvc.tick();
        DoorStateSvc.tick();
        delay(TICK_DELAY);
    }
}