/*
  Based on work by Tim Senecal.
  For Nano Every. (I have had problems with clones stopping working and requiring sketch to be reloaded.)
  Displays 2 streams of data on 0.96 inch oLED display which is visible in sunlight.
  Sends data over openLRS Passthru.
  To do this, open the openLRS configurator for TX , Set serial baudrate(bps) to 9600, data rate(bps) to 9600, Telemetry to "Yes" (This is Passthru)
  Connections to Hobbyking transmitter module:
  Identify serial port on module. (6 pins in a line.)
  Identify DTR pin. (Usually the left hand pin but it is marked on the back of the board.)
  Connect Arduino RX (Pin RXD1) --> TX pin on TX module (next to DTR pin)
  Connect Arduino Gnd --> Ground on TX module. (One of 2 pins furthest from DTR)
  Power supply: Arduino Vin --> Transmitter battery positive. (You can connect a flying lead into the TX module.)
  Arduino serial data default is, serial baud rate (9600) and 8 data bits, no parity,no stop,LSB and not inverted - the default
  Nano and display draw about 50mA
  30.5.2020 Experienced some overheating problems of Arduino so trying voltage level conversion. (3.3v VDD, 470 ohm resistors in series, 1k ohm resistor then connected to earth for SDA and SCL to convert to 3.3 input.) 4.7 k ohm pullup resistors also fitted for i2c requirement.
  29.10.2020. NOTE: oLED needs level conversion from 5v to 3.3v.IIC I2C Bi-Directional Logic Level Shifter Converter
  Module 5V to 3.3V from eBay works well(puretek-innovations 272249807035 )
  Details of OLED display from Banggood:
  White 0.96 Inch OLED I2C IIC Communication Display 128*64 LCD Module
  VCC: 3.3-5V
  GND: Ground
  SCL: Serial Clock
  SDA: Serial Data
  Size: 0.96 inch
  Resolution: 128*64
  Color: white
  Viewing angle: >160°
  Support platform: , 51 series, MSP430 series, STM32/2, chip, etc.
  Ultra low power consumption: 0.04W during normal operation
  Wide voltage support: DC 3.3V-5V
  Working temperature: -30 ° C ~ 80 ° C
  Driver chip: SSD1306
  Communication method: IIC, only 2 I/O ports
  Backlight: OLED self-illumination, no backlight required
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  RDF notes. Based on openLRS-receive-data-oLED-2.6
*/
