/*
* Name: clock,temp,piezo and ledring project
* Author: Hugo Karlsson
* Date: 2024-10-23
* Description: This project uses a ds3231 to measure time and displays the time on a 1306 OLED display.
* It also measures temperature with an analog temperature sensor and maps the value to a 9g-servo motor.
* The project uses a potentiometer to set a temperature threshold, controls a NeoPixel LED ring, and includes a piezo alarm.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>

#include "U8glib.h"

#include <Servo.h>

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 9  // Define pin for NeoPixel strip

// Init constants
const int termometer = A0;  // Analog pin for temperature sensor
const int potpin = A1;      // Analog pin for potentiometer

// Init global variables


// Construct objects
RTC_DS3231 rtc;                                                              // Real-time clock object
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);                               // OLED display object
Servo myservo;                                                               // Servo motor object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);  // NeoPixel LED strip object

/*
* Function: setup()
* Initializes the hardware, sets pin modes, and sets up the RTC and OLED display.
* Parameters: None
* Returns: void
*/
void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();                    // Start RTC
  u8g.setFont(u8g_font_unifont);  // Set font for OLED display
  pinMode(termometer, INPUT);     // Set thermometer pin as input
  myservo.attach(10);              // Attach servo motor to pin 9
  pinMode(potpin, INPUT);         // Set potentiometer pin as input

  for (int i = 4; i <= 7; i++) {
    pinMode(i, OUTPUT);  // Set pins 4 to 7 as output for alarm
  }
  strip.begin();            // Initialize NeoPixel strip
  strip.setBrightness(50);  // Set LED brightness
  strip.show();             // Initialize all pixels to 'off'

  //settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to compile time
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));  // Uncomment to set custom time
}

/*
* Function: loop()
* Main loop that runs continuously, updating the OLED display, servo motor, and LED ring.
* Parameters: None
* Returns: void
*/
void loop() {
  oledWrite(30, 20, getTime(), 30, 40, String(getTemp()), 30, 60, String(getthreshold()));  // Update OLED with time, temperature, and threshold
  servoWrite(getTemp());                                                                    // Move servo according to temperature
  colorwipe(strip.Color(0, 0, 0), 0);                                                       // Turn off all LEDs
  colorWipe();                                                                              // Set color on LED strip based on temperature and threshold
  delay(1000);                                                                              // Delay for 1 second
  //Serial.println(getTemp());
  //Serial.println(getTime());
  //Serial.println(getthreshold());
}



/*
* Function: getTime()
* Reads the current time from the DS3231 RTC module and formats it as a string.
* Parameters: None
* Returns: String with time in hh:mm:ss format
*/
String getTime() {
  DateTime now = rtc.now();  // Get current time

  Serial.println("The time is:" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));  // Print time to serial monitor
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());  // Return formatted time as a string
}

/*
* Function: getTemp()
* Reads the analog value from the temperature sensor, calculates the temperature in Celsius,
* and prints it to the serial monitor.
* Parameters: None
* Returns: float - Temperature in Celsius
*/
float getTemp() {
  int Vo;
  float R1 = 10000;  // Resistance of R1 in the circuit
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  // Coefficients for Steinhart-Hart equation

  Vo = analogRead(termometer);                                 // Read analog value from temperature sensor
  R2 = R1 * (1023.0 / (float)Vo - 1.0);                        // Calculate resistance of thermistor
  logR2 = log(R2);                                             // Logarithm of resistance
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // Calculate temperature in Kelvin
  T = T - 273.15;                                              // Convert temperature from Kelvin to Celsius

  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" C");
  return T;  // Return temperature in Celsius
}

/*
* Function: oledWrite()
* Writes text to the OLED display at specified coordinates.
* Parameters: - x, y: Coordinates for the first string
*             - text: The first string to display
*             - x1, y1: Coordinates for the second string
*             - text1: The second string to display
*             - x2, y2: Coordinates for the third string
*             - text2: The third string to display
* Returns: void
*/
void oledWrite(int x, int y, String text, int x1, int y1, String text1, int x2, int y2, String text2) {
  u8g.firstPage();  // Start the OLED drawing process

  do {
    u8g.drawStr(x, y, text.c_str());     // Draw the first string
    u8g.drawStr(x1, y1, text1.c_str());  // Draw the second string
    u8g.drawStr(x2, y2, text2.c_str());  // Draw the third string

  } while (u8g.nextPage());  // Continue drawing on next page
}

/*
* Function: servoWrite()
* Maps the temperature value to a corresponding angle on the servo motor and writes the angle to the servo.
* Parameters: - value: The temperature value to map
* Returns: void
*/
void servoWrite(float value) {
  int angle = map(value, 10, 40, 0, 180);  // Map temperature (10-40°C) to servo angle (0-180 degrees)
  myservo.write(angle);                    // Write the angle to the servo
}

/*
* Function: getthreshold()
* Reads the potentiometer value and maps it to a temperature threshold.
* Parameters: None
* Returns: int - Threshold temperature in the range 10 to 40°C
*/
int getthreshold() {
  return map(analogRead(potpin), 0, 1023, 10, 40);  // Map potentiometer value (0-1023) to temperature range (10-40°C)
}

/*
* Function: alarm()
* Plays a short tone on pins 4 to 7 to act as an alarm.
* Parameters: None
* Returns: void
*/
void alarm() {
  for (int i = 4; i <= 7; i++) {
    tone(i, 500);  // Play tone on pin i
    delay(50);     // Short delay
    noTone(i);     // Stop tone on pin i
  }
}

/*
* Function: colorwipe()
* Turns off all LEDs on the NeoPixel strip by wiping the strip with the specified color.
* Parameters: - c: Color to set
*             - wait: Delay between wiping each LED
* Returns: void
*/
void colorwipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);  // Set pixel color to 'c'
    strip.show();               // Update the strip
  }
}

/*
* Function: colorWipe()
* Updates the NeoPixel LED strip based on the current second and temperature threshold.
* LEDs turn blue if temperature is below or equal to threshold, red otherwise.
* Parameters: None
* Returns: void
*/
void colorWipe() {
  DateTime now = rtc.now();  // Get current time
  for (int i = 0; i < map(now.second(), 0, 59, 0, 23); i++) {
    if (getTemp() <= getthreshold()) {
      strip.setPixelColor(i, strip.Color(0, 255, 255));  // Blue if temperature <= threshold
    } else {
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red if temperature > threshold
    }
    strip.show();  // Update the strip
  }
}
