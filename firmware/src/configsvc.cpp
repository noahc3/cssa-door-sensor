#include "configsvc.hpp"

#include <Arduino.h>
#include <EEPROM.h>

#include "ledsvc.hpp"

ConfigData ConfigManager::load() {
    ConfigData data;

    EEPROM.begin(sizeof(data));
    EEPROM.get(0, data);
    EEPROM.end();

    return data;
}

void ConfigManager::save(ConfigData data) {
    LEDSvc.set(COLOR_GREEN, 100);

    Serial.printf("Size: %d", sizeof(data));

    EEPROM.begin(sizeof(data));
    EEPROM.put(0, data);
    EEPROM.commit();
    EEPROM.end();
}

void ConfigManager::clear() {
    ConfigData data;

    LEDSvc.set(COLOR_YELLOW, 100);

    data.wifimode = 0;
    strcpy(data.ssid, "");
    strcpy(data.userid, "");
    strcpy(data.password, "");
    strcpy(data.webhook, "");
    save(data);
}