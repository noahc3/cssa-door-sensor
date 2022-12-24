# CSSA Door Sensor

IoT Arduino project to detect the open/close status of the CSSA lounge door and ping a Discord webhook with the status.

## Software

### Firmware

The firmware for the ESP32 chip is written in C++ managed by the PlatformIO VSCode extension. The firmware can be flashed to the ESP32 as-is and can be configured through software via the USB serial interface.

## Hardware

### Components

This project uses a NodeMCU-32S clone sold by KeeYees [(Amazon link)](https://www.amazon.ca/dp/B07QCP2451) with the following additional hardware:

- 1x HC-SR04 Ultra Sonic Distance Sensor
- 1x 5mm RGB Common Cathode LED
- 1x SPDT On-Off-On toggle switch

### Pinout

Pinout configuration can be modified in the `constants/pins.hpp` file. Below illustrates the default pinout configuration.

Notes:
- The KeeYees board has the same pinout as the 38pin ESP32 Wroom DevKit reference PCB.
- The pinout choices are designed to support a 400-point compact breadboard.
- Do not connect any hardware to pins used by the flash controller or UART interface.

![](https://github.com/noahc3/cssa-door-sensor/raw/master/resources/pinout.png)