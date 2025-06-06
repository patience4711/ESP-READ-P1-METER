// * * * test serial port * * * 
void testMessage() {
char * inMessage = {0};
consoleOut("performing loopback test on uart0");
 if (Serial.availableForWrite() > 9 )
    {
        Serial.write("T");
        Serial.write("E");
        Serial.write("S");
        Serial.write("T"); 
        Serial.write("-");
        Serial.write("m");
        Serial.write("e");
        Serial.write("e");                
        Serial.write("\n");
        if (diagNose) ws.textAll("Serial wrote testdata");
    } else {
      consoleOut("Serial not available");
      return;
    }

     //now read the answer
     consoleOut("reading Serial");
     waitSerialAvailable(1);  
     
     if( Serial.available() ) 
     {
        int inByte = Serial.read();
        Serial.write(inByte);
   } else { consoleOut("Serial has no data");}
}


// function to show debug info
void consoleOut(String toLog) {
// decide to log to serial or the console 
  if(diagNose) 
  {
    Serial.println(toLog);
  } 
}

// flash the led
void ledblink(int i, int wacht) {
  for(int x=0; x<i; x++) {
    digitalWrite(led_onb, LED_AAN);
    delay(wacht);
    digitalWrite(led_onb, LED_UIT);
    delay(wacht);
   }
}


String getChipId(bool sec) {
    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  if(sec) return String(chipId); else return "ESP32C3-" + String(chipId);
}

//int FindCharInArray(char array[], char c, int len) {
//  for (int i = len - 1; i >= 0; i--) {
//    if (array[i] == c) {
//      return i;
//    }
//  }
//  return -1;
//}

long getValidVal(long valNew, long valOld, long maxDiffer)
{
  //check if the incoming value is valid
      if(valOld > 0 && ((valNew - valOld > maxDiffer) && (valOld - valNew > maxDiffer)))
        return valOld;
      return valNew;
}




char *split(char *str, const char *delim)
{
    char *p = strstr(str, delim);

    if (p == NULL)
        return NULL; // delimiter not found

    *p = '\0';                // terminate string after head
    return p + strlen(delim); // return tail substring
}


double round3(double value) {
   return (int)(value * 1000 + 0.5) / 1000.0;
}
double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}
double round1(double value) {
   return (int)(value * 10 + 0.5) / 10.0;
}
double round0(double value) {
   return (int)(value + 0.5) ;
}

bool waitSerialAvailable(int howMany) // wait untill something 's available
{
ledblink(howMany, 30);
Serial.println("waitSerialAvailable");
    for(int y=0; y < howMany; y++) {
     ledblink(2, 20);        
     Serial.println("y = " + String(y) );
      unsigned long wait = millis();
      while ( !Serial1.available() )
          {
          if ( millis() - wait > 2000) break; // after 2000 milis time out
          }
          // when timed out we check
      if ( Serial1.available() ) return true;
           
    }
  return false;
}

void empty_serial() { // remove any remaining data in serial buffer
  while(Serial.available()) {
    Serial.read();
    //uart_read_bytes(UART_NUM_2, &data, 1, 20 / portTICK_PERIOD_MS);
  }
}

void empty_Serial1() { // remove any remaining data in serial buffer
  while(Serial.available()) {
    Serial1.read();
  }
}
//File file = root.openNextFile();
//          int total = 0;
//          while(file){
//             total += file.size();
//             Serial.println("File: " + String(file.name()) + " size: " + String(file.size()) ); 
//             file = root.openNextFile();
void showDir() {
    ws.textAll("**** Content filesystem ****");
    char temp[50]={0};
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
          sprintf(temp, "%s  size %u<br>" , file.name(), file.size() );
          ws.textAll(String(temp));
          delay(100);
          file = root.openNextFile();
    }
 }

 void printFiles() {  
      consoleLog(" print existing files ******");
      for (int x=1; x < 13; x++) 
      {
          String bestand = "/monthly_vals" + String(x) + ".str";
          printStruct(bestand, x);
      }
        Serial.println("the current values are:");
        Serial.println("ECON_LT = " + String(ECON_LT));     
        Serial.println("ECON_HT = " + String(ECON_HT));
        Serial.println("ERET_LT = " + String(ERET_LT));
        Serial.println("ERET_HT = " + String(ERET_HT));
        Serial.println("mGAS = " + String(mGAS));     
}
