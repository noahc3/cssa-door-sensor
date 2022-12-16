#pragma once

#include <Arduino.h>

class LEDUtils {
  private:
    ulong lastTick;
    bool state;
    int counter;
    int interval;
    int r;
    int g;
    int b;
    void update();
    float brightness;

  public:
    LEDUtils();
    void tick();
    void set(int, int, int);
    void set(int, int, int, int);
    void setBrightness(float);
};

extern LEDUtils LED;