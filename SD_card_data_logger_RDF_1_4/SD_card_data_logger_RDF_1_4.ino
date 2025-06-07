/*
  Name:      SD card data logger
  Date:       3/6/2025
  Brief:      SD card data logger with RTC and oLED display
  Author:     David Forrest

***********************************************************************************************
 Revisions:
  Date        rev,    who       What
  June 25     1.4     DF       Put in oLED display. Works. This code is just a template tobe adapted as required.

// From:https://www.youtube.com/watch?v=tTp43eLqytA

// ADC1 data from pin A3
// ADC2 data from pin A2

  The circuit to connect via SPI:
    SD card attached to SPI bus as follows:
 ** MOSI - pin D11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin D12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin D13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
     Pin 4 used here for consistency with other Arduino examples
     Pin D10 actually used for CS. RDF note.

10 k pullup resistors fitted for i2c link.

// https://github.com/greiman/SSD1306Ascii   RDF Nice simple oLED routines. oLED Test for minimum program size.
// Edit AVRI2C_FASTMODE in SSD1306Ascii.h to change the default I2C address.

RDF based on Adafruit library RTClib based on sketch ds1307SqwPin
DS3231 to Arduino I2C connections: 
SDA to A4
SCL to A5
SQW to ground of LED, say

*/
