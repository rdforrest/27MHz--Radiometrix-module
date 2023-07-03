/*
  Name:       Radiometrix-TX
  Date:       29 December 2022
  Brief:      Radio-control at 27 MHz using Radiometrix modules
  Author:     David Forrest

***********************************************************************************************
  Revisions:
  Date        rev,    who      what
  Jan 23      1.3     DF        Reduce baud rate to 300
  Jan 23      1.7     DF        Single servo channel and data filter for testing
  Feb 13      2.7     DF        Use test data
  Feb 15      2.8     DF        Manchester encoding used
  Feb 21      3.0     DF        Now working on the bench with 6 servo channels and failsafes.
  Mar 3       3.1     DF        Put in timer and debug at Jonathan's suggestion.
  Mar 7       3.2     DF        Cleaned up code and put Failsafe in based on TX ID code
  Jul 3       3.3     DF        Revisited stick mapping
  
  Arduino serial data default is, serial baud rate (9600) and 8 data bits, no parity,one stop bit,LSB and not inverted - the default
  ppm reader for transmitter "buddy box" output.
  ppmReaderAccurate-1.1
  Thanks to Jonathan Askey for introducing this to me. RDF
  Fast and accurate PPM reader
  (Original work used: https://github.com/dfvella/ppm-handler)

  Note values will need to be halved to bring them back to real world values

  Source
  https://forum.arduino.cc/index.php?topic=85603.0

  Note due to increased accuracy values are read to 0.5us
  To get the array values back to real world full 'us' values the array value must be divided by 2

  Radiometrix modules here:
  http://www.radiometrix.com/lmt0-0


 v
  Hardware connections#
  - For TX only, connect TX power at 9 volts to VIN on Arduino
  - For TX only, use pin D8 as ppm input from TX
  - For TX only, use pin D6 TX for the serial output to the Radiometrix module through a 120 k ohm resistor to protect input
  - For TX only, 5v sounder on pin D7
  - For TX only, Transmitter output voltage to VIN on Nano
  - For TX only, GND from Transmitter to GND on Nano
  - For RX only, Servo outputs pins D5 to D10
  - For RX only, D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.
  - For RX only, A4 connected to module RSSI output
  - For RX only, D4 connected to Superbright LED with 120 ohm in series. This provides an LED beacon triggered by channel 4

  Manchester Encoding:
  https://mchr3k.github.io/arduino-libs-manchester/

  Radiometrix modules here:
  http://www.radiometrix.com/lmt0-0



  IIC I2C Bi-Directional Logic Level Shifter Converter Module 5V to 3.3V
  puretek-innovations (12579 ) (eBay)

  Note - ICR = the "Input Capture Register"

  RDF using serial procedures:
  https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

  Example 6 - Receiving binary data

  // Send Data Packet(Preamble+Start+Framing+Data Payload+Checksum. Based on:
  // http://www.radiometrix.com/files/additional/your-data-over-a-simple-radio-link.pdf

*/
