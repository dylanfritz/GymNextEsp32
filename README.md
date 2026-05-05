# GymNextEsp32
A lightweight REST API for the ESP32 that controls the GymNext Flex Timer over BLE.


This project turns an ESP32 into a BLE client that connects to the GymNext Flex Timer, sends its UART‑style command protocol, and exposes a simple HTTP REST interface for controlling the display.

It acts as a bridge between Wi‑Fi and the Flex Timer’s BLE command set, allowing external systems to send messages, switch modes, flash alerts, update the welcome screen, and more without the official GymNext app.

This project also aims to enable developers to write custom applications that interface with a GymNext Flex Timer to extend the functionality of the device.
