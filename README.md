# CSSA Door Sensor

IoT Arduino project to detect the open/close status of the CSSA lounge door and ping a Discord webhook with the status.

## Overview

### Device Operation

Device can be configured by connecting to a computer via USB and monitoring the serial output. Here you can set the WiFi network settings and target webhook URL. If no input is received within 10 seconds, the device will exit setup and continue startup.

Under normal operation, the device has three modes: 
- AUTO (Switch UP): Use the sensor to detect if the door is open or closed.
- MANUAL CLOSED (Switch MIDDLE): Override the sensor with CLOSED status.
- MANUAL OPEN (Switch DOWN): Override the sensor with OPEN status.

There is a delay between physical state changing and an update being sent to the webhook. This helps prevent erroneous sensor readings from falsely triggering a state change, and also gives supervisors a chance to change the operating mode if they do not want to send a status change notification.

### Status LED

The device has an LED to monitor device status without connecting to the serial interface:

| Color | Reason |
| -- | -- |
| Solid Red | Device has started booting |
| Solid Orange | Device is in the serial setup menu |
| Solid Green | Door is OPEN (manual or auto) |
| Solid Blue | Door is CLOSED (manual or auto) |
| Blinking Yellow | Device is writing config to flash storage |
| Blinking Cyan | Device is attempting to connect to WiFi |
| Blinking Purple | Device is performing an internet connection test |
| OFF | Device is in firmware flash mode (onboard LED should be solid red) |

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

![](https://github.com/noahc3/cssa-door-sensor/raw/master/resources/breadboard.jpg)