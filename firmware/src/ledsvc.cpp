#include "ledsvc.hpp"

#include <Arduino.h>

#include "constants/pins.hpp"

LEDService::LEDService() {
    pinMode(Pins::RED, OUTPUT);
    pinMode(Pins::GREEN, OUTPUT);
    pinMode(Pins::BLUE, OUTPUT);
}

void LEDService::tick() {
    if (interval > 0) {
        ulong tick = millis();
        ulong delta = tick - lastTick;
        counter += delta;
        if (counter >= interval) {
            counter = 0;
            state = !state;
            update();
        }

        lastTick = tick;
    }
}

void LEDService::set(int r, int g, int b) {
    LEDService::r = r;
    LEDService::g = g;
    LEDService::b = b;
    LEDService::interval = 0;
    LEDService::counter = 0;
    LEDService::lastTick = millis();
    state = true;
    update();
}

void LEDService::set(int r, int g, int b, int interval) {
    LEDService::set(r, g, b);
    LEDService::interval = interval;
}

void LEDService::update() {
    analogWrite(Pins::RED, r * state * brightness);
    analogWrite(Pins::GREEN, g * state * brightness);
    analogWrite(Pins::BLUE, b * state * brightness);
}

void LEDService::setBrightness(float brightness) {
    LEDService::brightness = brightness;
}