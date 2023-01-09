#include "serialutils.hpp"
#include <Arduino.h>

static int SerialUtils::readInt(int timeout) {
    int result = 0;
    int timeoutRemaining = timeout;
    while (Serial.available() == 0) {
        delay(100);
        if (timeout != 0) {
            timeoutRemaining -= 100;
            if (timeoutRemaining <= 0) {
                return -1;
            }
        }
    }

    return Serial.read() - '0';
}

static String SerialUtils::readString(bool hideInput) {
    String result = "";
    char read = '\0';
    while (true) {
        while (Serial.available() == 0) {
            delay(10);
        }

        read = (char)Serial.read();

        if (read == '\b') {
            Serial.print("\b \b");
            result = result.substring(0, result.length() - 1);
        } else if (read == '\n' || read == '\r') {
            Serial.println();
            break;
        } else {
            result += read;
            Serial.print(hideInput ? '*' : read);
        }
    }

    while (Serial.peek() == '\n' || Serial.peek() == '\r') {
        Serial.read();
    }

    return result;
}