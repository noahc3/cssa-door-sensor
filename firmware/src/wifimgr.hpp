#pragma once

#include <Arduino.h>

class WifiManager {
  private:
    bool internetTest();
    bool connectStandard(String, String);
    // bool connectEnterprise(String, String, String);
  public:
    WifiManager();

    bool connTestStandard(String, String);
    // bool connTestEnterprise(String, String, String);
    void saveStandard(String, String);
    // void saveEnterprise(String, String, String);
    bool connect();
};

extern WifiManager WifiMgr;