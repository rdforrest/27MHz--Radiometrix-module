/*
  Name:       Radiometrix-RX
  Date:       29 December 2022
  Brief:      Radio-control at 27 MHz using Radiometrix modules
  Author:     David Forrest

***********************************************************************************************
  Revisions:
  Date        rev,    who      what
  Dec 22      1.2     DF        Put in Servo software
   Arduino serial data default is, serial baud rate (9600) and 8 data bits, no parity,one stop bit,LSB and not inverted - the default

  Hardware connections#
  - For TX only, connect TX power at 9 volts to VIN on Arduino
  - For TX only, use pin D8 as ppm in
  - For TX only, use pin D6 TX for the serial output to Telemetry !
  - For TX only, 5v sounder on pin D7
  - For TX only, Transmitter output voltage to VIN on Nano
  - For TX only, GND from Transmitter to GND on Nano
  - For RX only, Servo outputs pins D5 to D10

  IIC I2C Bi-Directional Logic Level Shifter Converter Module 5V to 3.3V
  puretek-innovations (12579 ) (eBay)

  To convert array to servo values:
  ServoTimer2
  https://github.com/nabontra/ServoTimer2

   Hardware connections
 *  * use pin 8 as ppm in

   Note - ICR = the "Input Capture Register"
*/
