# 27MHz--Radiometrix-module. The LMT0 and LMR0 modules used simply. Development of this project.   
July 2023

Digital version with Manchester encoding now developed and with underwater trials on a model submarine. This digital version controls 6 servo channels and has built in failsafes. 

To connect the modules to an Arduino Nano. For TX, Arduino pin D6 connected to the Radiometrix module through a 120 k ohm resistor to protect the module input.
For RX, Arduino pin D11 connected to module data output (open collector) with 4.7K ohm pullup to +5v.

The RSSI display and the audio output on the RX module are very useful. The modules are CE approved and hence legal for r/c use.

I am not going to continue with any trials for the time being because it is not as reliable or easy to use as my existing  OpenLRS and LoRa gear working on 458 Mhz and I don't want to risk my subs any more.

June 2025
Rob Schuckman in Holland has been doing some very good development work, which now focusses on the use of Radiometrix modules rather than the AX5043. (Radiometrix modules are easliy available and CE approved.
You can see details of his work over here: https://robschuckman.wixsite.com/robschuckman/27mhz-remote-control
