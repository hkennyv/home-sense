# esp32-firmware
author(s): khuynh

## Overview

This directory contains the firmware (or prototype firmware) that will be used
with the microcontroller. Currently, I'm prototyping with an ESP32 but am not
sure if this will ultimately be the microcontroller I will use.

I hope to use this firmware at least as a sample for interfacing with the
various peripherals and sensors. That will hopefully carry over to whichever
microcontroller I decide to use in the end.

## Roadmap

* [ ] get BME280 task working
    * [ ] with temperature
    * [ ] with humidity
    * [ ] with pressure
* [ ] get accelerometer working
* [ ] get OLED LCD working
* [ ] get [ANCS](https://developer.apple.com/library/archive/documentation/CoreBluetooth/Reference/AppleNotificationCenterServiceSpecification/Introduction/Introduction.html) working