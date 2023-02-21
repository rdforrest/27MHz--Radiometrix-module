# 27MHz--Radiometrix-module. Uses the LMT0 and LMR0 modules. Early development of this project.  Core routines for conversion of PPM to digital values at TX and RX PWM outputs to servos (6 channels) and Failsafes are in place and working. RSSI works and is used to trigger an LED and the failsafe on the RX. 

Manchester encoding being tried. Controls 6 servo channels. Next thing is range testing.

The Modules also work with a direct feed from the TX ppm stream which makes very simple hardware. The servos are fed from the RX via JHMCU ppm to pwm convertor.

For TX, Arduino pin D6 connected to the Radiometrix module through a 120 k ohm resistor to protect input.
For RX, Arduino pin D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.

The Rssi display and the audio output on the RX module are very useful.
