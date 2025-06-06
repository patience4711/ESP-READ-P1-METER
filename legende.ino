// nodemcu pins
//  16  D0  
//   5  D1  
//   4  D2  
//   0  D3
//   2  D4   
//   GND
//   3,3v
//   14  D5  
//   12  D6  
//   13  D7 -> P02
//   15  D8 -> P03
//   3   D9
//   1   D10

// compile settings FS: Minima SPIFFS with OTA 
/* had to modyfy AsyncWebSynchronization.h to solve a compilation error

 lots of troubles geting data om serial1, getting wrong (chinese) characters
 tried everything, 
 different port pins, 
 the latest was reducing the cpu frequency to 80 (40 didn't work) 80 no result
 baud to 9600 didn't help
 remove the inverson on the pin didn't help
 Use the native serial port Serial like the nodemcu
 */
