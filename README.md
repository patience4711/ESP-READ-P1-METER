# ESP-READ-P1-METER

The purpose of this project is to read data from a so called smart meter (model Sagecom T210 ESMR5) via its serial port. The program reads the data and display's it on its webinterface. In addition, the data is transmitted via http and mosquitto. So that we can process the data in our domotica systems like 'Domotics' to display graphs and control switches.<br>

![frontpage](https://github.com/patience4711/ESP-READ-P1-METER/assets/12282915/bb65cf1f-f6bf-4e1c-ae48-c379628f3a7a)<br>

I know this has been done before but since i have other projects which partially use the same software, it is only a small step to adapt it to a new function. So it inherits many nice features from the other projects. 

The program has a lot of smart features. All settings can be done via the webinterface. Because the ESP has only one reliable working hardware serial port, this port is dedicated to the serial communication with the p1 meter. For the debugging we can use a web console just like in my other projects where the serial port is dedicated to the zigbee module. In the console we can call some processes and watch the output. 
See the [WIKI](https://github.com/patience4711/ESP-READ-P1-METER/wiki/GENERAL) for information on building it, the working, etc. 

This program runs on a nodemcu but in future there will be a version for ESP32.

## status
The software has been tested with a fake telegram that is fed to the serial port with a loopback wire. That works good so the basic 'engine' is oke. I am waiting for a 6-core wire with the rj11 that i ordered. Then i can test with my own meter (Sagemcom T210).

I uploaded the code that takes care of reading the telegram, it explains how this works.

## links
Here are some links to the projects where i got my inspiration (thanks to all for the good work they did.)
 * http://domoticx.com/arduino-p1-poort-telegrammen-uitlezen/
 * http://domoticx.com/p1-poort-slimme-meter-hardware/
 * https://github.com/romix123/P1-wifi-gateway/blob/main/src/P1WG2022current.ino
 * http://www.gejanssen.com/howto/Slimme-meter-uitlezen/#mozTocId935754

## compile this sketch
You can use the provided binary but if you must compile it yourself: Use arduino ide with the esp822 version 2.7.1 installed under boardmanager. The ota updates won't work with other versons.
<br>Please note: by popular demand I have published the code here but i do not feel responsible for problems as to compiling. Impossible for me to know why it doesn't compile in your situation.

## downloads
july 26 2023: There is a new version 0_b available.<br> 
Download [ESP-P1METER-v0_b](https://github.com/patience4711/ESP-READ-P1-METER/blob/main/ESP-P1METER-v0_b.bin)<br>

<br>In case someone wants to print the housing, here is an [stl file](https://github.com/patience4711/read-APSystems-YC600-QS1-DS3/blob/main/ESP-ECU-housing.zip)
This is for a nodemcu board 31x58mm.

## features
- Simply to connect to your wifi
- automatic polling or on demand via mqtt or http
- data can be requested via http and mosquitto
- data is displayed on the frontpage, as a monthly report and as the original telegram.
- Fast asyc webserver
- a very smart on-line console to send commands and debugging
- Smart timekeeping
- A lot of system info on the webpage

## the hardware
It is nothing more than an esp device like nodemcu, wemos or its relatives. The other materials are
- a prepared cable with an 6-pins RJ-11 plug.
- a 10K resistor to pullup the RX pin on the meter.
- optional a capacity (to buffer the 5v supply from the meter).
- optional a signal inverter (required for some type of p1 meters)

For info on how to build and use it, please see the <a href='https://github.com/patience4711/read-APSystems-YC600-QS1-DS3/wiki'>WIKI</a>

## how does it work
The P1-meter spits out data every 10 seconds, this has the form of a textdocument called a telegram. This document consists of lines that each represent a value.
It starts with a "/" and ends with a "!". The program reads the serial port until the "/" is found. Now the next incoming bytes are stored in a char array until the endcharacter is encountered. 
Next the checksum calculation is done and when the char array is approved, the interesting values can be extracted.

## changelog ##
