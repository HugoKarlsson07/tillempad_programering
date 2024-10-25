Clock, Temperature, LED Ring, and Servo Control Project
Author: Hugo Karlsson
Date: October 23, 2024
Project Overview
This Arduino-based project combines a real-time clock (RTC), temperature sensor, servo motor, NeoPixel LED ring, OLED display, and piezo alarm to display time and temperature, control an LED ring based on temperature, and activate an alarm at a set threshold.

Key features include:

Timekeeping with the DS3231 RTC module.
Displaying time and temperature on a 1306 OLED display.
Temperature measurement with an analog temperature sensor.
Temperature-to-angle mapping for a 9g servo motor.
NeoPixel LED ring color control based on temperature.
Configurable temperature threshold with a potentiometer.
Piezo alarm for threshold breaches.
Hardware Used
DS3231 RTC for timekeeping
1306 OLED display for visual output
Analog temperature sensor to measure temperature
9g Servo motor to display temperature as an angle
NeoPixel LED ring (60 LEDs)
Potentiometer for threshold configuration
Piezo buzzer for alarms
Software Components
This project utilizes the following libraries:

RTClib: For DS3231 communication and time functions
Wire: For I2C communication
U8glib: For OLED display control
Servo: For servo motor control
Adafruit_NeoPixel: For LED ring control
Pin Assignments
A0: Temperature sensor analog input
A1: Potentiometer analog input
PIN 9: NeoPixel LED ring control
PIN 10: Servo motor control
PINs 4–7: Piezo alarm control
Setup and Usage
Hardware Initialization:

Set up communication with peripherals (RTC, OLED).
Initialize pins and configure the RTC to the current time or a custom time.
Set LED brightness and attach the servo motor.
Main Loop (loop function):

Continuously updates the OLED display with the current time, temperature, and threshold.
Adjusts the servo motor based on the current temperature.
Sets LED ring color based on temperature relative to the threshold (blue if below or equal, red if above).
Delays each update cycle by 1 second for smooth operation.
Key Functions:

getTime(): Returns the current time as a formatted string.
getTemp(): Reads the temperature sensor and calculates temperature in Celsius.
oledWrite(): Displays time, temperature, and threshold on the OLED screen.
servoWrite(): Maps temperature to servo angle.
getthreshold(): Reads potentiometer value and maps it to a temperature threshold.
alarm(): Activates a piezo buzzer tone for alert.
colorWipe(): Updates LED colors based on the current time and temperature threshold.
Installation
Download or clone this repository.
Install the required Arduino libraries:
RTClib
U8glib
Servo
Adafruit_NeoPixel
Compile and upload the code to your Arduino board.
License
This project is open-source. Feel free to use, modify, and distribute it as needed.

