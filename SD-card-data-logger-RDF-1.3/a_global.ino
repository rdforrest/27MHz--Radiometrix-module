#include <OneWire.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
const int chipSelect = 10 ; //cs or the save select pin from the sd shield is connected to 10.
RTC_DS1307 RTC;
float celsius, fahrenheit;
