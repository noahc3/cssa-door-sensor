#include <Arduino.h>

struct ConfigData {
    char wifimode;
    char ssid[64];
    char userid[64];
    char password[64];
    char webhook[192];
};

class ConfigManager {
  public:
    ConfigData load();
    void save(ConfigData);
    void clear();
};

extern ConfigManager ConfigMgr;