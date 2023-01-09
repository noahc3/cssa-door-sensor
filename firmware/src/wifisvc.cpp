#include "wifisvc.hpp"
#include "configsvc.hpp"
#include "ledsvc.hpp"
#include "esp_wpa2.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define CONNECT_RETRIES 20
#define INTERNET_TEST_TIMEOUT 10000
#define INTERNET_TEST_URL "https://www.google.com"

WifiService::WifiService() {}

bool WifiService::connTestStandard(String ssid, String password)
{
    Serial.println("Performing connection test...");
    return connectStandard(ssid, password);
}

bool WifiService::connTestEnterprise(String ssid, String username, String password) {
    Serial.println("Performing connection test...");
    return connectEnterprise(ssid, username, password);
}

bool WifiService::connectStandard(String ssid, String password)
{
    int retries = 0;

    LEDSvc.set(COLOR_CYAN, 300);

    Serial.print("Connecting to the wireless network...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED && retries < CONNECT_RETRIES)
    {
        delay(500);
        Serial.print(".");
        retries++;
    }

    Serial.println("!");

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.printf("Could not connect to the access point '%s'\n", ssid);
        return false;
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return internetTest();
}

bool WifiService::connectEnterprise(String ssid, String username, String password)
{
    int retries = 0;

    LEDSvc.set(COLOR_CYAN, 300);

    Serial.print("Connecting to the wireless network...");

    WiFi.mode(WIFI_STA);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)username.c_str(), username.length()));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username.c_str(), username.length()));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password.c_str(), password.length()));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_enable());
    WiFi.begin(ssid.c_str());

    while (WiFi.status() != WL_CONNECTED && retries < CONNECT_RETRIES)
    {
        delay(500);
        Serial.print(".");
        retries++;
    }

    Serial.println("!");

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.printf("Could not connect to the access point '%s'\n", ssid);
        return false;
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return internetTest();

}

bool WifiService::internetTest()
{
    HTTPClient client;

    Serial.println("Testing internet connection...");

    LEDSvc.set(COLOR_PURPLE, 300);

    client.setTimeout(INTERNET_TEST_TIMEOUT);
    client.begin(INTERNET_TEST_URL);
    int code = client.GET();
    bool success = code >= 200 && code < 400;

    if (success)
    {
        Serial.println("Connection test passed");
    }
    else
    {
        Serial.println("Could not reach the internet.");
        Serial.printf("Status code: %d", code);
    }

    return success;
}

bool WifiService::connect()
{
    ConfigData data = ConfigSvc.load();
    bool res = false;
    if (data.wifimode == 1)
    {
        res = connectStandard(data.ssid, data.password);
    }
    else if (data.wifimode == 2) {
        res = connectEnterprise(data.ssid, data.userid, data.password);
    }
    else
    {
        Serial.println("No Wifi credentials found.");
    }

    return res;
}

void WifiService::saveStandard(String ssid, String password)
{
    Serial.println("Saving Wifi credentials to flash...");

    ConfigData data = ConfigSvc.load();
    data.wifimode = 1;
    strcpy(data.ssid, ssid.c_str());
    strcpy(data.password, password.c_str());

    ConfigSvc.save(data);
}

void WifiService::saveEnterprise(String ssid, String username, String password) {
    Serial.println("Saving Wifi credentials to flash...");
    
    ConfigData data = ConfigSvc.load();
    data.wifimode = 2;
    strcpy(data.ssid, ssid.c_str());
    strcpy(data.userid, username.c_str());
    strcpy(data.password, password.c_str());

    ConfigSvc.save(data);
}