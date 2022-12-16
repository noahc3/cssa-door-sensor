#pragma once

#include "WifiMgr.hpp"

class SetupManager {
  private:
    void menu();
    void wifi();
    void webhook();
    void clear();

  public:
    SetupManager();

    void start();
};

extern SetupManager SetupMgr;