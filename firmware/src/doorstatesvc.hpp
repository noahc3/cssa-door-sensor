#pragma once

#include <Arduino.h>

class DoorStateService {
  private:
    byte querySwitchMode();
    bool querySensorState();
    void tickSwitch();
    void tickSensor();
    

  public:
    DoorStateService();

    void tick();
    bool getState();
    String getStateString();
};

extern DoorStateService DoorStateSvc;