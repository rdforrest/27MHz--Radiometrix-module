/*
  Name:       Radiometrix-RX
  Date:       29 December 2022
  Brief:      Radio-control at 27 MHz using Radiometrix modules
  Author:     David Forrest

***********************************************************************************************
  Revisions:
  Date        rev,    who      what
  Dec 22      1.2     DF        Put in Servo software
  Jan 23      1.3     DF        Reduce baud rate to 300
  Feb 15      2.8     DF        Manchester encoding used
  Feb 18      3.2     DF        Gone back to Servo rather than Servo2 (Timer 2 clash in Manchester.)
  Feb 21      3.4     DF        Now working on the bench with 6 servo channels and failsafes.
  Feb 23      3.5     DF        Minor tweaks to RSSI Failsafe setting, servo range (still needs attention) etc. OK for model sub testing.

  Arduino serial data default is, serial baud rate (9600) and 8 data bits, no parity,one stop bit,LSB and not inverted - the default

  Hardware connections#
  - For TX only, connect TX power at 9 volts to VIN on Arduino
  - For TX only, use pin D8 as ppm in from TX
  - For TX only, use pin D6 TX for the serial output to the Radiometrix module through a 120 k ohm resistor to protect input
  - For TX only, 5v sounder on pin D7
  - For TX only, Transmitter output voltage to VIN on Nano
  - For TX only, GND from Transmitter to GND on Nano
  - For RX only, Servo outputs pins D5 to D10
  - For RX only, D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.
  - For RX only, A4 connected to module RSSI output

  Manchester Encoding:
  https://mchr3k.github.io/arduino-libs-manchester/

  Radiometrix modules here:
  http://www.radiometrix.com/lmt0-0

  IIC I2C Bi-Directional Logic Level Shifter Converter Module 5V to 3.3V
  puretek-innovations (12579 ) (eBay)

  To convert array to servo values:
  ServoTimer2 (NOT USED because of Timer 2 clash with Manchester.)
  https://github.com/nabontra/ServoTimer2
  This used instead:
  https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep

  Note - ICR = the "Input Capture Register"

  RDF using serial procedures: (NOT USED IN LATER VERSIONS)
  https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

  Example 6 - Receiving binary data
*/
