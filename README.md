# ESP32-C3-READ-P1-METER

The purpose of this project is to read data from a so called smart meter (model Sagecom 210 ESMR5) via its serial port. All we have to do is connect an ESP32C3 device (tested with ESP32C3 super mini) via an rj11 cable to the serial port of the meter. (I use a cable that is 15m long). The data is made available via mosquitto or an API and is also displayed on the frontpage of the webinterface.  Now we can process the data in our domotica systems like 'Domotics' to display graphs, control switches or manage a homebattery.<br>

![frontpage](https://github.com/patience4711/ESP-READ-P1-METER/assets/12282915/bb65cf1f-f6bf-4e1c-ae48-c379628f3a7a)<br>

I know this has been done before but not on this platform. The esp32c3 is very small and can be fed with the 5v of the p1 meter. Since i have other projects which partially use the same software, it is only a small step to adapt it to a new function. So it inherits many nice features from the other projects. 

The program has a lot of smart features. All settings can be done via the webinterface. Because the ESP has only one reliable working hardware serial port, this port is dedicated to the serial communication with the p1 meter. For the debugging we can use a web console just like in my other projects where the serial port is dedicated to the zigbee module. In the console we can call some processes and watch the output. 
See the [WIKI](https://github.com/patience4711/ESP-READ-P1-METER/wiki/GENERAL) for information on building it, the working, etc. 

This program runs on a nodemcu but in future there will be a version for ESP32-C3, not all ESP's can be powered by the meter.

## status and todo's
The software has been tested on a Sagemcom T210 meter and works good.
The next step is make it work with other meters. This is hard because i cannot test. The software is capable of reading the telegrams of other meters if they have an "open collector" port. The signal inversion is done in the software so no signal inverter is needed.

Since the esp32c3 has bluetooth capablities, i can invest how to make use of that for this project.

## links
Here are some links to the projects where i got my inspiration (thanks to all for the good work they did.)
 * http://domoticx.com/arduino-p1-poort-telegrammen-uitlezen/
 * http://domoticx.com/p1-poort-slimme-meter-hardware/
 * https://github.com/romix123/P1-wifi-gateway/blob/main/src/P1WG2022current.ino
 * http://www.gejanssen.com/howto/Slimme-meter-uitlezen/#mozTocId935754

## compile this sketch
<br>Please note: by popular demand I have published the code here but i do not feel responsible for problems as to compiling. Impossible for me to know why it doesn't compile in your situation.

## downloads
june 25 2025: There is a new version 0_1 available.<br> 
Download [ESP32C3-P1METER-v0_2](https://github.com/patience4711/ESP32-C3-READ-P1-METER/blob/main/ESP32C3-P1METER-v0_2.ino.esp32c3.bin)<br>

## features
- Simply to connect to your wifi
- automatic polling (frequency configurable) or on demand via mqtt or http
- data can be requested via http and mosquitto
- data is displayed on the frontpage, as a monthly report.
- we can see the telegram sent by the meter.
- Fast asyc webserver.
- a very smart on-line console to send commands and debugging.
- Smart timekeeping.
- A lot of system info on the webpage.
- the format of th data output is compatible with the home wizzard p1dongle

## the hardware
It is nothing more than an esp32c3 device. The other materials are
- a prepared cable with an 6-pins RJ-11 plug.
- a 4K7 resistor to pullup the RX pin on the meter.
For info on how to build and use it, please see the <a href='https://github.com/patience4711/read-APSystems-YC600-QS1-DS3/wiki'>WIKI</a>

## how does it work
The P1-meter has a so called open collector output. This requires that this port must be pulled up to get the signal right. We do this with a resistor, connected to rx. Rx is used to trigger the meter by making it high. So when we want data the tx pin is also high When the rxpin is pulled high. Now the meter starts spitting out data. This has the form of a textdocument called a telegram. This document consists of lines that each represent a value.
It starts with a "/" and ends with a "!". The telegram is spit out when the meter's rx is pulled high and by connecting the resistor, the output is also pulled high.

The program makes rx high and reads the serial port until the "/" is found. Now the next incoming bytes are stored in a char array until the endcharacter is encountered. So now we have the full telegram as a char array.
Next the checksum calculation is done and when the char array is approved, the interesting values can be extracted.

## changelog ##
version ESP32-C3-P1METER-v0_2:
  - cosmetical improvements
