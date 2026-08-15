# Project EVE

An ESP32-powered smart assistant device combining sensors, programming, displays, servo control, and automation into one interactive electronics system.

<img width="8064" height="6048" alt="IMG_9082" src="https://github.com/user-attachments/assets/e01c219e-c24f-48a9-ae87-eb21c60ff50d" />


<img width="3024" height="4032" alt="unnamed" src="https://github.com/user-attachments/assets/52920e9a-7067-4b75-86ef-8c9e9cb849f5" />


## What is EVE?

Project EVE is a hardware and software project I'm building to learn more about embedded programming, electronics, sensors, and engineering.

EVE currently uses an ESP32 to control an OLED display, ultrasonic sensor, servo motor, and buttons. The current version focuses on creating a radar-style scanning system that can detect objects and visualize their approximate position.

## Current Features

- OLED interface and animations
- Button-controlled menu
- Servo-controlled scanning
- Ultrasonic distance sensing
- Angle tracking
- Closest-object detection
- Radar-style visualization
- Fading radar detections
- Distance-based radar detection sizes
- 1° scanning
- 30°-150° scanning range
- Live system status monitoring
- Saved target visualization
- Target-lock system
- Automatic servo return to saved target angle
- OLED target-lock interface

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

**Current:** Radar scanning and visualization with 1° resolution, fading detections, saved target visualization, target locking, automatic servo targeting, and OLED status/lock interfaces

**Next:** Wi-Fi connectivity, web-based monitoring, final testing, and project polish

## Future Ideas

- Scan statistics
- Improved radar graphics
- Wi-Fi connectivity
- Web dashboard
- Additional sensors
- More advanced assistant features

## Development

Built using:

* Arduino IDE
* C/C++
* Adafruit GFX
* Adafruit SSD1306
* ESP32Servo

## AI Usage

AI tools were used as a development assistant during Project EVE. I used AI to help explain programming concepts, troubleshoot Arduino/ESP32 errors, and improve parts of the radar visualization. The hardware design, testing, debugging, implementation decisions, and final integration were done by me.

This project is a work in progress, and the repository will be updated as EVE develops.
