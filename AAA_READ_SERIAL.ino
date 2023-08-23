/* this program works as follows: first make the enable pin high
 * next make the serial buffer empty if there is data
 * mow wait untill data becomes available
 * start reading until the startsigb is found (likely this is the fst byte)
 * read the next bytes until the endsign is found.
 * if success the crc is checked and if oke the values are extracted from the telegram 
 * and send via mosquitto
 */

void meterPoll() {
  Serial.println("meterPoll");
  // if we don't have the testfiles we write them (in decode)
  digitalWrite(P1_ENABLE, HIGH); 
   if( read_into_array() ) {
      //we have a telegram
      
      digitalWrite(P1_ENABLE, LOW);
      decodeTelegram(); 
      sendMqtt(false);
      sendMqtt(true);
    } 
    // when done, write the logfile if not exists
    strcat(logChar, "\npoll done");
    if( !LittleFS.exists("/logChar.txt") ) {
          logCharsave(); // an existing logfile is not overwritten
    }
    // if the testFile still not exists we write it now
    if( !LittleFS.exists("/testFile.txt") ) {
         testFilesave(); // an existing logfile is not overwritten
    }
   Serial.println("meterPoll done");
   digitalWrite(P1_ENABLE, LOW);
}

bool read_into_array() {
    int byteCounter = 0;
    char rep[20]={0};
    char inByte[2];
    int Bytes=0;
    polled = false;
    // start waiting until serial available   
    waitSerialAvailable(5);
    // waste the serial buffer so that we start reading at the beginning of the telegram    
    empty_serial();
    // now we wait again until something is avialable
    if ( waitSerialAvailable(5) ) {
        memset(logChar, 0, sizeof(logChar));
        delayMicroseconds(250);
        memset(teleGram, 0, sizeof(teleGram));
        delayMicroseconds(250);
        strcat(logChar, "\nstart");
        while (Serial.available())
        {
              Serial.readBytes(inByte, 1);
              byteCounter ++;
              if (inByte[0] == '/') { 
                    sprintf(rep, "\nfound start at %d", byteCounter);
                    strcat(logChar,rep);
                    //Bytes = Serial.available(); // check how much is available
                    //sprintf(rep, "\navail %d", Bytes);
                    //strcat(logChar,rep);
                    strncat(teleGram, inByte, 1); 
                    // now we add the next 650 bytes to teleGram 
                    // until we encounter the endsign
                    for ( int x=0; x < 650; x++) {
                       Serial.readBytes(inByte, 1);
                       strncat( teleGram, inByte, 1);
                       // catch the endsign
                       if (inByte[0] == '!' ) {
                           console_Log("found the end sign");
                           strcat(logChar, "\nend sign");
                           // we need to read 4 more bytes (the crc) until the \n and then stop
                           Serial.readBytes(readCRC, 4);
                           strcat(teleGram, readCRC);
                           polled = true;
                           return true;
                        }   
                     }
               // if we are here, we read 650 characters more but no endsign has been found
               strcat(logChar,"\nfs no end");
               return false;              
           }

       // we terminate if more than 2000 bytes read
       if ( byteCounter > 2000 ) {
            strcat(logChar,"\n fs 2000 terminate");
            return false;       
           }
        }
   // if we are here, no startssign was found    
      strcat(logChar, "\nno startsign");
      return false;
   }
  // if we are here, no serial data was available
  strcat(logChar, "\nno data");
  return false;
}  

void decodeTelegram() {
      if (polled) {
        //we have a valid telegram, now we can decode it.
        //if no testfiles, we write them first
        if( !LittleFS.exists("/testFile.txt")) {
            testFilesave(); // an existing file is not overwritten
            //logCharsave(); // an existing file is not overwritten
        }
         int lengte = strlen(teleGram);
         console_Log("teleGram length = " + String(lengte));
         
         // the crc = calculated over the telegram inc start and endsign, so without crc
         // the teleGram contains the CRC so we terminate teleGram after the !
         teleGram[lengte - 4] = '\0';
         // now the teleGram is useless for testdecode so:
         testTelegram = false;
         int calculatedCRC = CRC16(0x0000, (unsigned char *) teleGram, lengte-4); 
         
         console_Log("the calculated crc = " + String(calculatedCRC));
      
         console_Log("strol of readCRC = " + String(strtol(readCRC, NULL, 16))); //8F46
    
        if(strtol(readCRC, NULL, 16) == calculatedCRC) //do the crc's match
        {
            console_Log("crc is correct, now extract values..");
            strcat(logChar, "\ncrc ok");
            extractTelegram();   
            polled = true;
            eventSend(2); // inform the webbpage that there is new data
            console_Log("polled true");
            return;
        } else {
            console_Log("crc is wrong, now extract values..");
            strcat(logChar, "\ncrc false");
            polled=false;
            console_Log("not polled");
            return;
        }
    }
    
}


void extractTelegram() {
/*
This function extracts the interesting values from the telegram as floats
therefor we call the function returnFloat
with arguments :len ( the line length excl *kWh
: the start of the number and : the length of the number
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
          ERET_HT = returnFloat(what, 20, 10, 10);
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

void sendMqtt(bool gas) {

if(Mqtt_Format == 0) return;  

  char Mqtt_send[26]={0};  
  strcpy(Mqtt_send, Mqtt_outTopic);
//  if( Mqtt_send[strlen(Mqtt_send)-1] == '/' ) {
//    strcat(Mqtt_send, String(Inv_Prop[which].invIdx).c_str());
//  }
  bool reTain = false;
  char pan[50]={0};
  char tail[40]={0};
  char toMQTT[512]={0};

// the json to p1 domoticz must be something like {"command":"udevice", "idx":1234, "svalue":"lu;hu;lr;hr;ac;ar"}
// the json to gas {"command":"udevice", "idx":1234, "svalue":"3.45"} 
//where lu is low tariff usage Wh, hu is high tariff usage  Wh), lr is low tariff return Wh), 
//hr is high tariff return  Wh, ac is actual power consumption (in W) and ar is actual return W .  
   switch( Mqtt_Format)  { 
    case 1: 
       if(!gas) {
        snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\"}" , el_Idx, ECON_LT*1000 , ECON_HT*1000, ERET_LT*1000, ERET_HT*1000, PACTUAL_CON, PACTUAL_RET);
       } else {
        snprintf(toMQTT, sizeof(toMQTT), "{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%.3f;\"}", gas_Idx, mGAS);
       }
       break;
    case 2:
       snprintf(toMQTT, sizeof(toMQTT), "{\"econ_lt\":%.2f,\"econ_ht\":%.2f,\"eret_ht\":%.2f,\"eret_lt\":%.2f,\"actualp_con\":%.2f,\"actualp_ret\":%.2f,\"gas\":%.3f}" , ECON_LT, ECON_HT, ERET_LT, ERET_HT, PACTUAL_CON, PACTUAL_RET, mGAS);
       break;
    case 3:
       snprintf(toMQTT, sizeof(toMQTT), "field1=%.3f&field2=%.3f&field3=%.3f&field4=%.3f&field5=%.0f&field6=%.0f&field7=%.3f&status=MQTTPUBLISH" ,ECON_LT, ECON_HT, ERET_LT, ERET_HT, PACTUAL_CON, PACTUAL_RET, mGAS);
       reTain=false;
       break;
     }

   // mqttConnect() checks first if we are connected, if not we connect anyway
   if(mqttConnect() ) MQTT_Client.publish ( Mqtt_send, toMQTT, reTain );
}
