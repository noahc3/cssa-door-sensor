#pragma once

#include <Arduino.h>

class DoorStateManager {
  private:
    byte querySwitchMode();
    bool querySensorState();
    void tickSwitch();
    void tickSensor();
    

  public:
    DoorStateManager();

    void tick();
    bool getState();
    String getStateString();
};

extern DoorStateManager DoorStateMgr;