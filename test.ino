void read_test() {
    int byteCounter = 0;
    char rep[20]={0};
    char inByte[2];
    int Bytes=0;
    polled = false;
    digitalWrite(P1_ENABLE, HIGH); 
    consoleLog("wait for Serial avalable");
    // start waiting until serial available   
    waitSerialAvailable(5);
    // waste the serial buffer so that we start reading at the beginning of the telegram    
    consoleLog("if bytes available we empty the buffer");
    if(Serial1.available()) {
      empty_Serial1();
      consoleLog("buffer Serial emptied");
    } else { consoleLog("nothing available after wait");}

    // now we wait again until something is available, that should be a new telegram
    consoleLog("wait again for data on Serial");
    if ( waitSerialAvailable(5) ) {
        memset(teleGram, 0, sizeof(teleGram));
        delayMicroseconds(250);
        if(Serial1.available() ) {
            consoleOut("1st number of bytes on Serial:  " + String(Serial1.available()));
            consoleOut("2nd number of bytes on Serial:  " + String(Serial1.available()));
            //consoleOut("3th number of bytes on Serial:  " + String(Serial.available()));
           // consoleOut("4th number of bytes on Serial:  " + String(Serial.available()));
            //consoleOut("5th number of bytes on Serial:  " + String(Serial.available()));
            } 
          } else {
          consoleOut("still nothing available, abort");
          return;
          }
          // if we are here there is something
          testPrint(4); //print the first 4 characters
             if(Serial1.available() > 102) {
               consoleOut("more than 100 available");
               testPrint(10);
              //String spatie="--";
              for ( int x=0; x < 101; x++) {
                      //int data = Serial.read(); //returns one byte as int
                       Serial1.readBytes(inByte, 1);
                       Serial.print(String(inByte[0]));
                       strncat( teleGram, inByte, 1);
                       //strncat( teleGram, String(data).c_str(), 1);
                       //strncat( teleGram, spatie.c_str(), 2);
                       byteCounter ++;
                   }
               consoleOut("100 bytes written in teleGram");
               consoleOut("telegram = " + String(teleGram) );
              } 

      consoleOut("bytecounter = " + String(byteCounter));
      polled = true;
      digitalWrite(P1_ENABLE, LOW);
}


void testPrint(int aantal) {
  String inputString = "";
  char inComing[2];
  consoleOut("print first 20 characters");
  for(int x=0; x<aantal; x++) {
  Serial1.read(inComing,1);
  //inputString += data;
  consoleOut(String(inComing));
  //consoleLog(inputString);
  }
}    
