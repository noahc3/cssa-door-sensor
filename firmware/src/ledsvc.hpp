#pragma once

#include <Arduino.h>

#define COLOR_RED 255, 0, 0
#define COLOR_ORANGE 255, 165, 0
#define COLOR_YELLOW 255, 255, 0
#define COLOR_GREEN 0, 255, 0
#define COLOR_BLUE 0, 0, 255
#define COLOR_PURPLE 128, 0, 128
#define COLOR_CYAN 0, 255, 255
#define COLOR_MAGENTA 255, 0, 255
#define COLOR_WHITE 255, 255, 255
#define COLOR_OFF 0, 0, 0

class LEDService {
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
    LEDService();
    void tick();
    void set(int, int, int);
    void set(int, int, int, int);
    void setBrightness(float);
};

extern LEDService LEDSvc;