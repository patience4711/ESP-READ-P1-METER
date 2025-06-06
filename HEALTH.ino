// *************************************************************************
//                   system healtcheck 
//**************************************************************************

//void healthCheck() {
//   
//    //memoryCheck();
//    
//    if(!timeRetrieved) { 
//      getTijd();
//      // if the time has changed we must update the webpage
//      // eventSend(1);
//    }
//    diagNose = false; // in case it was forgotten
//    // reset the errorcode so that polling errors remain
//    //if(errorCode >= 3000) errorCode -= 3000;
//    //if(errorCode >= 200) errorCode -= 200;
//    //if(errorCode >= 100) errorCode -= 100;
//}




// void memoryCheck()
// {
// // should we send the heap via mosquitto
//    if(Mqtt_Format != 0 && Mqtt_Format != 5 && Mqtt_stateIDX != 0) {
//       if (mqttConnect() ) {
//            char Mqtt_send[26];
//            strcpy( Mqtt_send, Mqtt_outTopic) ;
//            if( Mqtt_send[strlen(Mqtt_send)-1] == '/' ) {
//                 strcat(Mqtt_send, "heap");
//            }
//      
//        char toMQTT[50]={0}; // when i make this bigger a crash occures
//        
//        sprintf( toMQTT, "{\"idx\":%d,\"svalue\":\"%ld;%ld\"}", Mqtt_stateIDX, ESP.getFreeHeap(), ESP.getFreeContStack()); 
//        //must be like {"idx":901,"nvalue":0,"svalue":"22968;1904"}   length 44
//    
//        if(diagNose) ws.textAll("mqtt publish heap, mess is : " + String(toMQTT) );
//        MQTT_Client.publish ( Mqtt_send, toMQTT, false);  
//        
//        memset(&toMQTT, 0, sizeof(&toMQTT)); //zero out 
//        delayMicroseconds(250);
//       }  
//    }
// }
