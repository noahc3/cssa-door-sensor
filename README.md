# CSSA Door Sensor

IoT Arduino project to detect the open/close status of the CSSA lounge door and ping a Discord webhook with the status.

## Software

### Firmware

The firmware for the ESP32 chip is written in C++ managed by the PlatformIO VSCode extension. The firmware can be flashed to the ESP32 as-is and can be configured through software via the USB serial interface.

## Hardware

### Components

This project uses a NodeMCU-32S clone sold by KeeYees [(amazon link)](https://www.amazon.ca/dp/B07QCP2451) with the following additional hardware

- 1x HC-SR04 Ultra Sonic Distance Sensor
- 1x RGB LED
- 1x SPDT On-Off-On toggle switch *(not yet implemented)*
- 2x momentary push buttons *(not yet implemented)*

### Pinout

Pinout configuration can be modified in the `constants/pins.hpp` file. Below illustrates the default pinout configuration.

Notes:
- The KeeYees board has the same pinout as the 38pin ESP32 Wroom DevKit reference PCB.
- Do not connect any hardware to pins used by the flash controller or UART interface.

![](https://i.imgur.com/HRISoJM.png)