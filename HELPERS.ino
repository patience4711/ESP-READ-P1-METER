

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


// void swap_to_usb () {
//  delay(1000);
//  if (!USB_serial)
//  {
//      Serial.swap();
//      delay(300);
//      empty_serial(); // remove what's in the buffer
//      USB_serial=true;
//  } 
//}
//
//void swap_to_hw () {
//   delay(200); //was 300
//  if (USB_serial) 
//  {
//      Serial.swap();
//      delay(100);
//      empty_serial(); // remove what's in the buffer
//      USB_serial=false;
//   }
//}

char *split(char *str, const char *delim)
{
    char *p = strstr(str, delim);

    if (p == NULL)
        return NULL; // delimiter not found

    *p = '\0';                // terminate string after head
    return p + strlen(delim); // return tail substring
}


//float makeFloat(uint8_t bgn, uint8_t digits) { 
//  // like this it is normally decoded
//  char tail[16];
//  strncpy(tail, telegramLine + bgn, digits);
//  //Serial.println("tail= " + String(tail));
// // now we have the number, convert it to a float
//  return atof(tail);
//}

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
      while ( !Serial.available() )
          {
          if ( millis() - wait > 2000) break; // after 2000 milis time out
          }
          // when timed out we check
      if ( Serial.available() ) return true;
           
    }
}

void empty_serial() { // remove any remaining data in serial buffer
  while(Serial.available()) {
    Serial.read();
  }
}
