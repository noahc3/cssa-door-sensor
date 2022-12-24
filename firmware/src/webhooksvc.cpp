#include "webhooksvc.hpp"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "configsvc.hpp"
#include "constants/strings.hpp"

#define STATUS_CHANGE_DELAY_SECONDS 10

bool debouncedDoorState = false;
bool lastDoorState = false;
unsigned long lastStatusChange = 0;

WebhookService::WebhookService() {
    
}

void WebhookService::init(bool initialState) {
    sendMessage(Strings::STARTUP_MSG);
    debouncedDoorState = initialState;
    lastDoorState = initialState;
    sendDoorMessage(initialState);
}

void WebhookService::trySendMessage(bool doorState) {
    unsigned long timestamp = millis();
    if (doorState != lastDoorState) {
        lastDoorState = doorState;
        lastStatusChange = timestamp;
    }

    bool delayExceeded = timestamp - lastStatusChange > STATUS_CHANGE_DELAY_SECONDS * 1000;

    if (delayExceeded && lastDoorState != debouncedDoorState) {
        debouncedDoorState = lastDoorState;
        sendDoorMessage(debouncedDoorState);
    }
}

void WebhookService::sendDoorMessage(bool doorState) {
    sendMessage(doorState ? Strings::DOOR_OPEN_MSG : Strings::DOOR_CLOSED_MSG);
}

void WebhookService::sendMessage(String message) {
    DynamicJsonDocument doc(128);
    doc["content"] = message;

    String json;
    serializeJson(doc, json);

    Serial.println("Sending message: " + json);

    HTTPClient http;
    http.begin(ConfigSvc.getWebhookUrl());
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    http.end();

    Serial.println("Message sent, response code: " + String(httpCode));
}