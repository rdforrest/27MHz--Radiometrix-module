# 27MHz--Radiometrix-module. Uses the LMT0 and LMR0 modules. Early development of this project.   

Digital version with Manchester encoding now being tried. This digital version controls 6 servo channels and has built in failsafes. 

To connect the modules to an Arduino Nano. For TX, Arduino pin D6 connected to the Radiometrix module through a 120 k ohm resistor to protect the module input.
For RX, Arduino pin D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.

The RSSI display and the audio output on the RX module are very useful.

The Modules  also work with a direct feed from the TX ppm stream which makes for very simple hardware. This version has not been given any range or model testing to date. The TX side needs a 5v voltage regulator and the data should be put through a 120k resistor to protect the module. The servos are fed from the RX via JHMCU ppm to pwm convertor. This version needs separate failsafe units. Suitable ones (Thanks to Geofrancis for the suggestion) are:

https://www.ebay.co.uk/itm/401482463111
