#include "setupsvc.hpp"
#include "configsvc.hpp"
#include "ledsvc.hpp"
#include "serialutils.cpp"
#include "wifisvc.hpp"
#include <Arduino.h>

SetupService::SetupService() {}

void SetupService::start() { menu(); }

void SetupService::menu() {
    while (true) {
        LEDSvc.set(COLOR_ORANGE);
        ConfigData data = ConfigSvc.load();
        Serial.println("Current configuration:");
        Serial.printf("Wifi SSID: %s\n", data.ssid);
        if (data.wifimode == 2) {
            Serial.printf("Wifi user: %s\n", data.userid);
        }
        Serial.printf("Webhook: %s\n\n", data.webhook);

        Serial.println("Select option:");
        Serial.println("1. Configure Wifi");
        Serial.println("2. Set Webhook");
        Serial.println("3. Clear configuration");
        Serial.println("0. Resume boot");

        int option = SerialUtils::readInt(10000);

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
        case -1:
            return;
        default:
            Serial.println("Invalid option");
            break;
        }
    }
}

void SetupService::wifi() {
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
        password = SerialUtils::readString(true);
        Serial.printf("%s/%s\n", ssid, password);
        result = WifiSvc.connTestStandard(ssid, password);
        if (result) {
            WifiSvc.saveStandard(ssid, password);
        }
        break;
    case 2:
        Serial.print("SSID: ");
        ssid = SerialUtils::readString();
        Serial.print("User ID: ");
        userId = SerialUtils::readString();
        Serial.print("Password: ");
        password = SerialUtils::readString(true);
        result = WifiSvc.connTestEnterprise(ssid, userId, password);
        if (result) {
            WifiSvc.saveEnterprise(ssid, userId, password);
        }
        break;
    default:
        Serial.println("Invalid option");
        break;
    }
}

void SetupService::webhook() { 
    Serial.print("Webhook: ");
    String webhook = SerialUtils::readString();
    ConfigSvc.saveWebhookUrl(webhook);
}

void SetupService::clear() {
    Serial.println("Clearing configuration");
    ConfigSvc.clear();
}