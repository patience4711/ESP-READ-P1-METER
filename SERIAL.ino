void handle_Serial () {
    Serial.println("\n* * * handle_Serial, type LIST-COMMANDS");  
    int SerialInByteCounter = 0;
    char InputBuffer_Serial[100] = "";
    byte SerialInByte;  
    // first check if there is enough data, at least 13 bytes
    delay(200); //we wait a little for more data as the esp seems slow
    if(Serial.available() < 13 ) {
      // less then 13, we can't expect more so we give up 
      while(Serial.available()) { Serial.read(); } // make the buffer empty 
      Serial.println("invalid command, abort " + String(InputBuffer_Serial));
     return;
    }

// now we know there are at least 13 bytes so we read them
 while(Serial.available()) {
             SerialInByte=Serial.read(); 
             //Serial.print("+");
            
            if(isprint(SerialInByte)) {
              if(SerialInByteCounter<100) InputBuffer_Serial[SerialInByteCounter++]=SerialInByte;
            }    
            if(SerialInByte=='\n') {                                              // new line character
              InputBuffer_Serial[SerialInByteCounter]=0;
              //   Serial.println(F("found new line"));
             break; // serieel data is complete
            }
       }   
   Serial.println("InputBuffer_Serial = " + String(InputBuffer_Serial) );
   int len = strlen(InputBuffer_Serial);
   // evaluate the incomming data

     //if (strlen(InputBuffer_Serial) > 6) {                                // need to see minimal 8 characters on the serial port
     //  if (strncmp (InputBuffer_Serial,"10;",3) == 0) {                 // Command from Master to RFLink
  
          if (strcasecmp(InputBuffer_Serial, "LIST-COMMANDS") == 0) {
              Serial.println(F("*** AVAILABLE COMMANDS ***"));
              Serial.println(F("DEVICE-REBOOT;"));
              Serial.println(F("PRINTOUT-SPIFFS"));
              Serial.println(F("METERPOLL-TEST;"));
              Serial.println(F("DELETE-FILE=filename; (delete a file)")); 
                     
              return;
    } else 

   
    if (strcasecmp(InputBuffer_Serial, "METERPOLL-TEST") == 0) {  
        actionFlag=126;
        return;
    } else

    if (strcasecmp(InputBuffer_Serial, "DEVICE-REBOOT") == 0) {
           Serial.println("\ngoing to reboot ! \n");
           delay(1000);
           ESP.restart();
    } else

    if (strcasecmp(InputBuffer_Serial, "PRINTOUT-SPIFFS") == 0) {
          actionFlag=47;
          return;
    } else
         
    if (strncasecmp(InputBuffer_Serial, "DELETE-FILE=", 12 ) == 0) {  
       Serial.println("len = " + String(len));
       String bestand="";
       for(int i=12;  i<len+1; i++) { bestand += String(InputBuffer_Serial[i]); }
       Serial.println("bestand  = " + bestand);
         // now should have like /bestand.json or so;
         if (SPIFFS.exists(bestand)) 
         {
            Serial.println("going to delete file " + bestand); 
            SPIFFS.remove(bestand);
            Serial.println("file " + bestand + " removed!"); 
         } else { Serial.println("unkown file, forgot the / ??"); }
          return;           
   
      } else {
          
      Serial.println( String(InputBuffer_Serial) + " INVALID COMMAND" );     
     }

       
    // } // end if if (strncmp (InputBuffer_Serial,"10;",3) == 0)
    Serial.println( String(InputBuffer_Serial) + " UNKNOWN COMMAND" );
    //  end if strlen(InputBuffer_Serial) > 6
  // the buffercontent is not making sense so we empty the buffer
  empty_serial();
   //
}   
