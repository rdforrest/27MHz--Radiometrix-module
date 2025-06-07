#include <OneWire.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
const int chipSelect = 10 ; //cs or the save select pin from the sd shield is connected to 10.
RTC_DS1307 RTC;
float celsius, fahrenheit;

// oLED display. 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required. (NOT SURE IF NEEDED)
#define RST_PIN -1

/////////////////////////////////////////////////////////////////

// oLED display
SSD1306AsciiAvrI2c oled;
/////////////////////////////////////////////////////////////////