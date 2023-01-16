# 27MHz--Radiometrix-module. Uses the LMT0 and LMR0 modules. Early development of this project. Still no correct data transferred over wireless but data is correctly transferred via hardwire. Core routines for conversion of PPM to digital values at TX and RX PWM outputs to servos (6 channels) and Failsafes are in place and working but again only using a hardwire. RSSI works and is used to trigger an LED and the failsafe on the RX.
For TX, Arduino pin D6 connected to 470 and 1k ohm resistors as voltage divider connected to ground. Module data input tapped off at 1k ohm resistor.
For RX, Arduino pin D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.

