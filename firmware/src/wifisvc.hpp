#pragma once

#include <Arduino.h>

class WifiService {
  private:
    bool internetTest();
    bool connectStandard(String, String);
    bool connectEnterprise(String, String, String);
  public:
    WifiService();

    bool connTestStandard(String, String);
    bool connTestEnterprise(String, String, String);
    void saveStandard(String, String);
    void saveEnterprise(String, String, String);
    bool connect();
};

extern WifiService WifiSvc;