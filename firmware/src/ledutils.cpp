#include "ledutils.hpp"

#include <Arduino.h>

#include "constants/pins.hpp"

LEDUtils::LEDUtils() {
    pinMode(Pins::RED, OUTPUT);
    pinMode(Pins::GREEN, OUTPUT);
    pinMode(Pins::BLUE, OUTPUT);
}

void LEDUtils::tick() {
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

void LEDUtils::set(int r, int g, int b) {
    LEDUtils::r = r;
    LEDUtils::g = g;
    LEDUtils::b = b;
    LEDUtils::interval = 0;
    LEDUtils::counter = 0;
    LEDUtils::lastTick = millis();
    state = true;
    update();
}

void LEDUtils::set(int r, int g, int b, int interval) {
    LEDUtils::set(r, g, b);
    LEDUtils::interval = interval;
}

void LEDUtils::update() {
    analogWrite(Pins::RED, r * state * brightness);
    analogWrite(Pins::GREEN, g * state * brightness);
    analogWrite(Pins::BLUE, b * state * brightness);
}

void LEDUtils::setBrightness(float brightness) {
    LEDUtils::brightness = brightness;
}