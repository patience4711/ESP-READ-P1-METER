// the meter transmits every second so we could read until we find the start sign "/"
// And drop everything before that

bool readTelegram() {
/* this function tries to read the serial output from the meter into a char arry  
 * we can verify this with the crc 
 * if correct extract the values of interest
 * and we can serve the array at request
 */
      if(findStartinSerial() ) 
      { 
          readTelegramInArray();
          if(diagNose ) {
             ws.textAll("back in readTelegram");
             delay(100);
             ws.textAll(String(teleGram));
             delay(100);
             }
          // now we have an aray that contains the whole telegram.
          // And we have na array that contains the CRC
          // can we find a match
      } else {
         if(diagNose ) ws.textAll("no startsign found");
         return false;
      }
  
      console_Log( "length of testGram (incl !8F46 and a '\'= " + String(testLength) );
      if (diagNose) 
      {
      ws.textAll("length of testGram (incl !8F46 and a '\'= " + String(testLength) );
      delay(100);
      }
     int lengte = strlen(teleGram);
     console_Log("teleGram length = " + String(lengte));
  
     // now we have the array and can calculate the crc
     int calculatedCRC = CRC16(0x0000, (unsigned char *) teleGram, lengte); 
     console_Log("the calculated crc = " + String(calculatedCRC));
  
     console_Log("strol of readCRC = " + String(strtol(readCRC, NULL, 16))); //8F46

    // readCRC 8F46 should be 36678 
 
    if(strtol(readCRC, NULL, 16) == calculatedCRC) //do the crc's match
    {
    console_Log("crc is oke, decoding..");
    extractTelegram();
    sendMqtt(false); // send pi meter format to domoticz
    sendMqtt(true);  // send gas to domoticz
    return true;
    // we can keep the telegram to answer a request for it
    } else {
       console_Log("crc failed, exitting..");
       return false;
    }
}

bool findStartinSerial()
{
    // keep reading serial until the start sign is found. Normally this cannot take more than 10 seconds
    // we do this a couple of times. One cycle = 2,5 seconds so 5 times would be enough
    for (int z=0; z< 6; z++ ) { // this loop takes
      if( waitSerialAvailable() ) {  //this takes 2.5 seconds
          while ( Serial.available() ) {
              ESP.wdtDisable();
              //can we read the bytes until the start sign?
              if (Serial.read() == '/') { 
                  ESP.wdtEnable(1);
                  console_Log("found startsign");
//                  if(diagNose) {
//                     ws.textAll("found startsign");
//                     delay(100); 
//                     }
                  return true;
              }
          }

       }
       // so if we are here, we did'nt found the start sign.
     
       }
       return false;
}

void readTelegramInArray() 
{
// this function reads the serial port int an char array
// it is called when the startsign has been found 
char inByte[2];
teleGram[0]='/'; // add the startsign; this character has been read already

        while (Serial.available() )
        {
        ESP.wdtDisable();
        Serial.readBytes(inByte, 1);
        if (inByte[0] == '!' ) {
           strncat( teleGram, inByte, 1);
           console_Log("found the end sign");
           // we need to read 4 more bytes (the crc) untill the \n and then stop
           Serial.readBytes(readCRC, 4);
           ESP.wdtEnable(1);
           return;
        } else { 
          strncat (teleGram, inByte, 1);
        }

    if(Serial.available() < 10 ) send_testGramChunk(); // for testing: make the serial buffer full again 
    
    }     
        ESP.wdtEnable(1);
}




void extractTelegram() {
/*
extract the interesting values out of the telegram as floats
therefor we call the function returnFloat
with arguments len ( the line length excl *kWh
the start of the number and the length of the number
*/    
char what[24];
    // find 1-0:1.8.1(000051.775*kWh) len = 20
      strcpy(what, "1-0:1.8.1(");
      if(strstr(teleGram, what )) {
          ECON_LT = returnFloat(what, 20, 10, 10);
          console_Log("extracted ECON_LT = " + String(ECON_LT, 3));
      }  
    // find 1-0:1.8.2(000000.000*kWh) len = 20
      strcpy(what, "1-0:1.8.2(");
      if(strstr(teleGram, what )) {
          ECON_HT = returnFloat(what, 20, 10, 10);
          console_Log("extracted ECON_HT = " + String(ECON_HT, 3));
     }
    // find 1-0:2.8.1(0000524.413*kWh) len = 20
      strcpy(what, "1-0:2.8.1(");
      if(strstr(teleGram, what )) {
          ERET_LT = returnFloat(what, 20, 10, 10);
          console_Log("extracted ERET_LT = " + String(ERET_LT, 3));
    }  
    // find 1-0:2.8.2(000000.000*kWh) len = 20
      strcpy(what, "1-0:2.8.2(");
      if(strstr(teleGram, what )) {
          float ERET_HT = returnFloat(what, 20, 10, 10);
          console_Log("extracted ERET_HT = " + String(ERET_HT, 3));
    }
    // find 1-0:1.7.0(00.335*kW) len=16 start 10 count 6
      strcpy(what, "1-0:1.7.0(");
      if(strstr(teleGram, what )) {
          PACTUAL_CON = returnFloat(what, 16, 10, 6) * 1000; // watts
          console_Log("extracted PACTUAL_CON = " + String(PACTUAL_CON, 3));
     } 
    // find 1-0:2.7.0(00.000*kW) len=16 start 10 count 6
      strcpy(what, "1-0:2.7.0(");
      if(strstr(teleGram, what )) {
          PACTUAL_RET = returnFloat(what, 16, 10, 6) * 1000; //watts
          console_Log ("extracted PACTUAL_RET = " + String(PACTUAL_RET, 3));         
      }
    // find 0-1:24.2.1(171105201000W)(00016.713*m3) len 39 start 26 count 9
      strcpy(what, "0-1:24.2.1");
      if(strstr(teleGram, what )) {
          mGAS = returnFloat(what, 39, 26, 9);
          console_Log ("extracted mGAS = " + String(mGAS, 3));        
      }
}


float returnFloat(char what[24], uint8_t len, uint8_t bgn, uint8_t count) { 
   char extract[len+1];
   char number[16];
   strncpy(extract, strstr(teleGram, what), len);
   // now we have an array starting with the line that contains 'what'
   // Serial.println("extract = " + String(extract));
   // we copy the characters representing the value in tail
   strncpy(number, extract + bgn, count);
   //  Serial.println("tail= " + String(tail));
   // now we have the number, convert it to a float
   return atof(number);
}

void console_Log(String toLog) {
  if(diagNose)
  {
    ws.textAll(toLog);
    delay(100);
  }
}  
