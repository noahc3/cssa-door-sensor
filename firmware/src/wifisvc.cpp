#include "wifisvc.hpp"
#include "configsvc.hpp"
#include "ledsvc.hpp"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define CONNECT_RETRIES 20
#define INTERNET_TEST_TIMEOUT 10000
#define INTERNET_TEST_URL "https://www.google.com"

WifiService::WifiService() {}

bool WifiService::connTestStandard(String ssid, String password) {
    Serial.println("Performing connection test...");
    return connectStandard(ssid, password);
}

bool WifiService::connectStandard(String ssid, String password) {
    int retries = 0;

    LEDSvc.set(COLOR_CYAN, 300);

    Serial.print("Connecting to the wireless network...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED && retries < CONNECT_RETRIES) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    Serial.println("!");

    if (WiFi.status() != WL_CONNECTED) {
        Serial.printf("Could not connect to the access point '%s'\n", ssid);
        return false;
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return internetTest();
}

bool WifiService::internetTest() {
    HTTPClient client;

    Serial.println("Testing internet connection...");

    LEDSvc.set(COLOR_PURPLE, 300);

    client.setTimeout(INTERNET_TEST_TIMEOUT);
    client.begin(INTERNET_TEST_URL);
    int code = client.GET();
    bool success = code >= 200 && code < 400;

    if (success) {
        Serial.println("Connection test passed");
    } else {
        Serial.println("Could not reach the internet.");
        Serial.printf("Status code: %d", code);
    }

    return success;
}

bool WifiService::connect() {
    ConfigData data = ConfigSvc.load();
    bool res = false;
    if (data.wifimode == 1) {
        res = connectStandard(data.ssid, data.password);
    } else {
        Serial.println("No Wifi credentials found.");
    }

    return res;
}

void WifiService::saveStandard(String ssid, String password) {
    Serial.println("Saving Wifi credentials to flash...");

    ConfigData data = ConfigSvc.load();
    data.wifimode = 1;
    strcpy(data.ssid, ssid.c_str());
    strcpy(data.password, password.c_str());

    ConfigSvc.save(data);
}