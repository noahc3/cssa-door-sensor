#include "setupmgr.hpp"
#include "configmgr.hpp"
#include "ledutils.hpp"
#include "serialutils.cpp"
#include "wifimgr.hpp"
#include <Arduino.h>

SetupManager::SetupManager() {}

void SetupManager::start() { menu(); }

void SetupManager::menu() {
    while (true) {
        LED.set(255, 153, 0);
        ConfigData data = ConfigMgr.load();
        Serial.println("Current configuration:");
        Serial.printf("Wifi SSID: %s\n", data.ssid);
        Serial.printf("Webhook: %s\n\n", data.webhook);

        Serial.println("Select option:");
        Serial.println("1. Configure Wifi");
        Serial.println("2. Set Webhook");
        Serial.println("3. Clear configuration");
        Serial.println("0. Resume boot");

        int option = SerialUtils::readInt();

        switch (option) {
        case 1:
            wifi();
            break;
        case 2:
            webhook();
            break;
        case 3:
            clear();
            break;
        case 0:
            return;
        default:
            Serial.println("Invalid option");
            break;
        }
    }
}

void SetupManager::wifi() {
    String ssid;
    String userId;
    String password;
    bool result;
    Serial.println("Wifi network type:");
    Serial.println("1. Standard");
    Serial.println("2. WPA2 Enterprise (User ID and Password)");

    int option = SerialUtils::readInt();

    switch (option) {
    case 1:
        Serial.print("SSID: ");
        ssid = SerialUtils::readString();
        Serial.print("Password: ");
        password = SerialUtils::readString();
        Serial.printf("%s/%s\n", ssid, password);
        result = WifiMgr.connTestStandard(ssid, password);
        if (result) {
            WifiMgr.saveStandard(ssid, password);
        }
        break;
    case 2:
        Serial.print("SSID: ");
        ssid = SerialUtils::readString();
        Serial.print("User ID: ");
        userId = SerialUtils::readString();
        Serial.print("Password: ");
        password = SerialUtils::readString();
        break;
    default:
        Serial.println("Invalid option");
        break;
    }
}

void SetupManager::webhook() { Serial.println("Webhook"); }

void SetupManager::clear() {
    Serial.println("Clearing configuration");
    ConfigMgr.clear();
}