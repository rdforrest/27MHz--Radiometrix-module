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


  Hardware connections#
  - For TX only, connect TX power at 9 volts to VIN on Arduino
  - For TX only, use pin D8 as ppm in
  - For TX only, use pin D6 TX for the serial output to Telemetry !
  - For TX only, 5v sounder on pin D7
  - For TX only, Transmitter output voltage to VIN on Nano
  - For TX only, GND from Transmitter to GND on Nano
  - For RX only, Servo outputs pins D5 to D10
  - For RX only, Servo software serial output to pin D11

  For TX, Arduino pin D6 connected to 470 and 1k ohm resistors as voltage divider connected to ground. Module data input tapped off at 1k ohm resistor. For RX, Arduino pin D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.

  IIC I2C Bi-Directional Logic Level Shifter Converter Module 5V to 3.3V
  puretek-innovations (12579 ) (eBay)

  Note - ICR = the "Input Capture Register"

  RDF using serial procedures:
  https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

  Example 6 - Receiving binary data

  // Send Data Packet(Preamble+Start+Framing+Data Payload+Checksum. Based on:
  // http://www.radiometrix.com/files/additional/your-data-over-a-simple-radio-link.pdf

*/
