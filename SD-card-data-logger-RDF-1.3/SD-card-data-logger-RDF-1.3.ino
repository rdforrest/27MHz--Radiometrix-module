// From:https://www.youtube.com/watch?v=tTp43eLqytA
// ADC1 data from pin A3
// ADC2 data from pin A2

/*
  SD card test

  This example shows how use the utility libraries on which the'
  SD library is based in order to get info about your SD card.
  Very useful for testing a card when you're not sure whether its working or not.

  The circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
     Pin 4 used here for consistency with other Arduino examples
     Pin 10 actually used for CS. RDF note.


  created  28 Mar 2011
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe


// RDF based on Adafruit library RTClib based on sketch ds1307SqwPin
// DS3231 to Arduino connections: SDA to A4,SCL to A5, SQW to ground of LED say

*/
