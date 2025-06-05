// find out if the request comes from inside the network
//check if the first 9 characters of the router's ip ( 192.168.0 ) is in the url
bool checkRemote(String url) {
   if(securityLevel==0) return false; //no security so never remote
   if ( url.indexOf(WiFi.gatewayIP().toString().substring(0, securityLevel)) == -1 ) return true; else return false;
//return false;
}

// we come here when an unknown request is done

void handleNotFound(AsyncWebServerRequest *request) {
  
bool intern = false;
if(!checkRemote( request->client()->remoteIP().toString()) ) intern = true;

// **************************************************************************
//             R E S T R I C T E D   A R E A
// **************************************************************************
// access only from inside the network

if ( intern ) {    //DebugPrintln("the request comes from inside the network");
        String serverUrl = request->url().c_str();             
        if ( serverUrl.indexOf("/API/TELEGRAM") > -1) {
            char tozend[1024];
            strcat(teleGram, "\r\npolled at ");
            strcat(teleGram, timeStamp);
            testTelegram = false; // useless for testing   
            request->send(200, "text/plain", String(teleGram) );
        }

        if ( serverUrl.indexOf("/API/poll") > -1) {
              if(Polling)
              {
                 request->send ( 200, "text/plain", "automatic polling, skipping" ); //zend bevestiging
                 return; 
              }

             String teZenden = F("polled p1 meter ");
             request->send ( 200, "text/plain", teZenden ); 
             actionFlag = 26; // takes care for the polling
             return;
       }
       
       
       // if we are here, no valid api was found    
       request->send ( 200, "text/plain", "ERROR this link is invalid, go back <--" );//send not found message
       }             

    else { // not intern
      //DebugPrint("\t\t\t\t unauthorized, not from inside the network : ");
      request->send ( 200, "text/plain", "ERROR you are not authorised, go back <--" );//send not found message
    }
}
