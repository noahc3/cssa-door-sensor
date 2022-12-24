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
        password = SerialUtils::readString();
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
        password = SerialUtils::readString();
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