bool getTijd() {

  timeRetrieved = false; // stays false until time is retrieved  
  timeClient.begin();
  //unsigned long epochTime = 0;
  //get the time, if fails we try again during healthcheck

  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
    // we have to do this conditional, if time retrieving failed
    if (epochTime < 1000) {
    ntpUDP.stop();
    return false;
  } else {
   
    epochTime += atoi(gmtOffset) * 60;
    setTime(epochTime); // dit moeten we doen omdat anders zomertijd() niet werkt
    if ( zomerTijd == true ) {
      if (zomertijd() == true) {  
        epochTime += 3600; // een uur erbij
        setTime(epochTime);
      }
    }
    timeRetrieved=true;  
    #ifdef LOG 
    Update_Log(1, "got time"); 
    #endif
    }
  
    ntpUDP.stop();
 // the time is retrieved and saved in setTime
    datum = day();

   delay(10);
   sun_setrise(); //to calulate moonshape sunrise etc. and the switchtimes

   return true;
}
