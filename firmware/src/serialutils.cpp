#include "serialutils.hpp"
#include <Arduino.h>

static int SerialUtils::readInt() {
    int result = 0;
    while (Serial.available() == 0) {
        delay(100);
    }

    return Serial.read() - '0';
}

static String SerialUtils::readString() {
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
            Serial.print(read);
        }
    }

    while (Serial.peek() == '\n' || Serial.peek() == '\r') {
        Serial.read();
    }

    return result;
}