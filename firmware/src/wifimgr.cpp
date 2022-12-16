#include "wifimgr.hpp"
#include "configmgr.hpp"
#include "ledutils.hpp"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define CONNECT_RETRIES 20
#define INTERNET_TEST_TIMEOUT 10000
#define INTERNET_TEST_URL "https://www.google.com"

WifiManager::WifiManager() {}

bool WifiManager::connTestStandard(String ssid, String password) {
    Serial.println("Performing connection test...");
    return connectStandard(ssid, password);
}

bool WifiManager::connectStandard(String ssid, String password) {
    int retries = 0;

    LED.set(0, 134, 179, 300);

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

bool WifiManager::internetTest() {
    HTTPClient client;

    Serial.println("Testing internet connection...");

    LED.set(122, 0, 179, 300);

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

bool WifiManager::connect() {
    ConfigData data = ConfigMgr.load();
    bool res = false;
    if (data.wifimode == 1) {
        res = connectStandard(data.ssid, data.password);
    } else {
        Serial.println("No Wifi credentials found.");
    }

    return res;
}

void WifiManager::saveStandard(String ssid, String password) {
    Serial.println("Saving Wifi credentials to flash...");

    ConfigData data = ConfigMgr.load();
    data.wifimode = 1;
    strcpy(data.ssid, ssid.c_str());
    strcpy(data.password, password.c_str());

    ConfigMgr.save(data);
}