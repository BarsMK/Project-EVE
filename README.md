# Project EVE

An ESP32-powered smart assistant device combining sensors, programming, displays, servo control, and automation into one interactive electronics system.

<img width="4032" height="3024" alt="IMG_8966" src="https://github.com/user-attachments/assets/983e6199-d28e-4b51-a7a5-cc6bc97029b7" />

## What is EVE?

Project EVE is a hardware and software project I'm building to learn more about embedded programming, electronics, sensors, and engineering.

EVE currently uses an ESP32 to control an OLED display, ultrasonic sensor, servo motor, and buttons. The current version focuses on creating a radar-style scanning system that can detect objects and visualize their approximate position.

## Current Features

*  OLED interface and animations
*  Button-controlled menu
*  Servo-controlled scanning
*  Ultrasonic distance sensing
*  Angle tracking
*  Closest-object detection
*  Radar-style visualization
*  Persistent object detections
*  Distance-based radar detection sizes

## Hardware

* ESP32 DevKit
* 0.96" OLED display
* HC-SR04 ultrasonic sensor
* Servo motor
* Push buttons
* Breadboard and jumper wires

## How It Works

The servo moves the ultrasonic sensor through a range of angles. At each angle, the ESP32 measures the distance to an object and stores the result.

The angle and distance are then converted into coordinates so the object can be displayed on the OLED radar.

```text
Servo + Ultrasonic Sensor
          ↓
      ESP32 Processing
          ↓
   Angle + Distance Data
          ↓
      OLED Radar
```

## Progress

EVE is being developed in stages, with each stage adding another part of the system.

**Current:** Radar scanning and visualization

**Next:** Target lock, scan statistics, and further interface improvements

## Future Ideas

* Target-lock system
* Scan statistics
* Improved radar graphics
* Wi-Fi connectivity
* Web dashboard
* Additional sensors
* More advanced assistant features

## Development

Built using:

* Arduino IDE
* C/C++
* Adafruit GFX
* Adafruit SSD1306
* ESP32Servo

This project is a work in progress, and the repository will be updated as EVE develops.
