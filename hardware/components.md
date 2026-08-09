# Project EVE — Hardware Components

## V0.1 Core Hardware

| Component                  | Purpose                                       |
| -------------------------- | --------------------------------------------- |
| ESP32 DevKit               | Main microcontroller                          |
| 0.96" SSD1306 OLED         | Displays EVE's interface, angle, and distance |
| Ultrasonic Distance Sensor | Measures the distance to objects              |
| Servo Motor                | Rotates the ultrasonic sensor during scanning |
| SELECT Button              | Selects menu options and returns to the menu  |
| MOVE Button                | Moves between menu options                    |
| Breadboard                 | Prototyping and connecting the electronics    |
| Jumper Wires               | Connecting components to the ESP32            |

## Current Pin Assignments

| Component       | ESP32 Pin |
| --------------- | --------- |
| Ultrasonic TRIG | GPIO 5    |
| Ultrasonic ECHO | GPIO 18   |
| SELECT Button   | GPIO 27   |
| Servo           | GPIO 13   |
| OLED SDA        | GPIO 21   |
| OLED SCL        | GPIO 22   |

## V0.1 Capabilities

The current hardware system can:

* Display EVE's boot sequence
* Display a main menu
* Navigate between SCAN and STATUS
* Measure distance using the ultrasonic sensor
* Rotate the ultrasonic sensor using a servo
* Scan between 30° and 150°
* Display the current angle and distance
* Return to the main menu using the SELECT button

## Notes

This is an early prototype. The hardware layout is currently being tested on a breadboard. Pin assignments and components may change as Project EVE develops.
