// the meter transmits every second so we could read until we find the start sign "/"
// And drop everything before that

bool readSerial(bool testModus) {
/* this function tries to read the serial output from the meter into a char arry  
 * we can verify this with the crc 
 * if correct extract the values of interest
 * and we can serve the array at request
 */
    if(findStartinSerial() ) 
    { 
        readTelegramToArray(testModus);
        
        console_Log("back in readTelegram");
        
        if(diagNose ) {
           ws.textAll(String(teleGram));
           delay(100);
         }
        // now we have an aray that contains the whole telegram.
        // And we have na array that contains the CRC
        // can we find a match ?
      } else {
         errorCode = 11;
         //console_Log("no startsign found");
         return false;
      }
  
     console_Log( "testGram length (incl !8F46 and a backslash) = " + String(testLength) );
     int lengte = strlen(teleGram);
     console_Log("teleGram length = " + String(lengte));
  
     // now we have the array and can calculate the crc
     int calculatedCRC = CRC16(0x0000, (unsigned char *) teleGram, lengte); 
     console_Log("the calculated crc = " + String(calculatedCRC));
  
     console_Log("strol of readCRC = " + String(strtol(readCRC, NULL, 16))); //8F46

    // readCRC 8F46 should be 36678 
 
    if(strtol(readCRC, NULL, 16) == calculatedCRC) //do the crc's match
    {
    console_Log("crc is oke, now extract values..");
    extractTelegram();
    sendMqtt(false); // send pi meter format to domoticz
    if (Mqtt_Format == 1) sendMqtt(true);  // send gas separate to domoticz
    polled = true;
    errorCode = 0;
    return true;
    
    } else {
       console_Log("crc failed, exitting..");
       polled = false;
       return false;
    }
}

bool findStartinSerial()
{
    // keep reading serial until the start sign is found. Normally this cannot take more than 10 seconds
    // we do this a couple of times. One cycle = 2,5 seconds so 5 times would be enough
    for (int z=0; z< 6; z++ ) { // this loop takes
      ESP.wdtDisable();
      yield();
      if( waitSerialAvailable() ) {  //this takes 2.5 seconds
          while ( Serial.available() ) {

              //can we read the bytes until the start sign?
              if (Serial.read() == '/') { 
                  ESP.wdtEnable(1);
                  console_Log("found startsign");
                  return true;
              }
          }
       }
     
    }
    ESP.wdtEnable(1);
    console_Log("no startsign found !!!");
    errorCode = 11;
    return false; // did not find the startsign
}

void readTelegramToArray(bool testModus) 
{
// this function reads the data from serial port into a char array
// it is called when the startsign has been found 
        // first cleanup
        memset(teleGram, 0, sizeof(teleGram)); //zero out 
        delayMicroseconds(250);
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
    // if we are testing we have to keep the serial buffer full
    if(testModus == true) {
        if(Serial.available() < 10 ) send_testGramChunk(); // for testing: make the serial buffer full again 
        }
    }     
        ESP.wdtEnable(1);
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
   // now we have an array 'extract' starting with the line that contains 'what'
   // Serial.println("extract = " + String(extract));
   // we copy the part representing the value to 'number'
   strncpy(number, extract + bgn, count);
   //  Serial.println("tail= " + String(tail));
   // now we have the number, convert it to a float and return
   return atof(number);
}

void console_Log(String toLog) {
  if(diagNose)
  {
    ws.textAll(toLog);
    delay(100);
  }
}

// ********************************************************************************
//                     send the values via mosquitto
// ********************************************************************************
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
