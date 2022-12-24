#pragma once

#include "WifiSvc.hpp"

class SetupService {
  private:
    void menu();
    void wifi();
    void webhook();
    void clear();

  public:
    SetupService();

    void start();
};

extern SetupService SetupSvc;