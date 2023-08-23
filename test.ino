//void findCRC() {
//char toCheck[1024];
//// the crc is calculated from the telegram incl start and endsign so excl the crc 
//// this seams to work when we copy that to a different array
//// so now we try to remove the last 4 characters
//
//       int len1 = strlen(teleGram);
//       console_Log("len1 = " + String(len1) );       
//
//       strncpy(readCRC, teleGram + len1-4, 4); 
//       console_Log("readCRC = " + String(readCRC) );
//
//       int len2 = len1 - 4 ;
//       
//       console_Log("len2 = " + String(len2) );
//       teleGram[len1-4] = '\0';
//       console_Log("teleGram to check = " + String(teleGram) );       
//       //int calculatedCRC = CRC16(0x0000, (unsigned char *) toCheck, len2);
//       int calculatedCRC = CRC16(0x0000, (unsigned char *) teleGram, len1-4);
//       console_Log("the calculated crc = " + String(calculatedCRC));
//      
//       console_Log("strol of readCRC = " + String(strtol(readCRC, NULL, 16))); //8F46
//  
//}

/* sends a test telegram via serial, we can read in via the loopback cable 
 *  we can call this via the console: 10;telegram
* this telegram comes from janssen

/Ene5\XS210 ESMR 5.0

1-3:0.2.8(50)
0-0:1.0.0(171105201324W)
0-0:96.1.1(4530303437303030303037363330383137)
1-0:1.8.1(000051.775*kWh)
1-0:1.8.2(000000.000*kWh)
1-0:2.8.1(000024.413*kWh)
1-0:2.8.2(000000.000*kWh)
0-0:96.14.0(0001)
1-0:1.7.0(00.335*kW)
1-0:2.7.0(00.000*kW)
0-0:96.7.21(00003)
0-0:96.7.9(00001)
1-0:99.97.0(0)(0-0:96.7.19)
1-0:32.32.0(00002)
1-0:32.36.0(00000)
0-0:96.13.0()
1-0:32.7.0(229.0*V)
1-0:31.7.0(001*A)
1-0:21.7.0(00.335*kW)
1-0:22.7.0(00.000*kW)
0-1:24.1.0(003)
0-1:96.1.0(4730303538353330303031313633323137)
0-1:24.2.1(171105201000W)(00016.713*m3)
!8F46

*/

//bool send_testGramChunk() {
//
//        // send chunks of this test telegram as large as the available space in serial
//        // www keeps track of where we were
//         
//        // this is a telegram with \r\n after each line Attention!! the backslash is doubled here (escaped)
//        //char testGram[1024]={"/Ene5\\XS210 ESMR 5.0\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(171105201324W)\r\n0-0:96.1.1(4530303437303030303037363330383137)\r\n1-0:1.8.1(000051.775*kWh)\r\n1-0:1.8.2(000000.000*kWh)\r\n1-0:2.8.1(000024.413*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.335*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00003)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(0)(0-0:96.7.19)\r\n1-0:32.32.0(00002)\r\n1-0:32.36.0(00000)\r\n0-0:96.13.0()\r\n1-0:32.7.0(229.0*V)\r\n1-0:31.7.0(001*A)\r\n1-0:21.7.0(00.335*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303538353330303031313633323137)\r\n0-1:24.2.1(171105201000W)(00016.713*m3)\r\n!8F46\r\n"};
//        char testGram[1024]={"/Ene5\\XS210 ESMR 5.0\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(171105201324W)\r\n0-0:96.1.1(4530303437303030303037363330383137)\r\n1-0:1.8.1(000050.111*kWh)\r\n1-0:1.8.2(000010.222*kWh)\r\n1-0:2.8.1(000025.333*kWh)\r\n1-0:2.8.2(000015.444*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.335*kW)\r\n1-0:2.7.0(00.555*kW)\r\n0-0:96.7.21(00003)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(0)(0-0:96.7.19)\r\n1-0:32.32.0(00002)\r\n1-0:32.36.0(00000)\r\n0-0:96.13.0()\r\n1-0:32.7.0(229.0*V)\r\n1-0:31.7.0(001*A)\r\n1-0:21.7.0(00.335*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303538353330303031313633323137)\r\n0-1:24.2.1(171105201000W)(00016.713*m3)\r\n!1635\r\n"};
//       
//        testLength = strlen(testGram);
//        
//        if(www >= testLength) return true;
//        
//        int bufspace = Serial.availableForWrite();
//        int end = www+bufspace;
//
//        for (int i=www; i < end; i++)
//        {
//           Serial.print( String(testGram[i]) );
//           www++;
//           if( www >= testLength ) { // all has been send
//              console_Log("testGram ready, www = " + String(www));
//           memset(testGram, 0, sizeof(testGram));
//           return true;
//           }
//        }
//    // if we are here, not all bytes of testGram are sent
//    //we wait some time until serial.available has updated
//    delay(100);
//    int avail = Serial.available(); 
//    console_Log("wrote " + String(bufspace) + " now available : " + String(avail) );
//    
//    return false;
//}
//
//bool send_testData() {
//  // first make the serial buffer emty
//        empty_serial();
//        // this message has the startsign and the end
//        char testGram[64]={"/if you see this, the serial port is oke!4861"};
//        int len = strlen(testGram);
//        int bufspace = Serial.availableForWrite();
//        console_Log(" available for write = " + String(bufspace) );
//        console_Log(" going to write bytes: " + String(len));
//        for (int i=0; i < len; i++)
//        {
//           Serial.print( String(testGram[i]) );
//        }
//    console_Log("wrote " + String(bufspace) + "bytes" );
//    return true;
//}
