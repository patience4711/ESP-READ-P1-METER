void start_asyserver() {

server.on("/CONSOLE", HTTP_GET, [](AsyncWebServerRequest *request){
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    request->send_P(200, "text/html", CONSOLE_HTML);
  });

// Simple Firmware Update
  server.on("/FWUPDATE", HTTP_GET, [](AsyncWebServerRequest *request){
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );    
    strcpy( requestUrl, "/" );
    if (!request->authenticate("admin", pswd) ) return request->requestAuthentication();
    request->send_P(200, "text/html", UPDATE_FORM); 
    });
  server.on("/handleFwupdate", HTTP_POST, [](AsyncWebServerRequest *request){
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    //swap_to_usb(); // this should work now
    Serial.println(F("FWUPDATE requested"));
    if( !Update.hasError() ) {
    toSend="<br><br><center><h2>UPDATE SUCCESS !!</h2><br><br>";
    toSend +="click here to reboot<br><br><a href='/REBOOT'><input style='font-size:3vw;' type='submit' value='REBOOT'></a>";
    } else {
    toSend="<br><br><center><kop>update failed<br><br>";
    toSend +="click here to go back <a href='/FWUPDATE'>BACK</a></center>";
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", toSend);
    response->addHeader("Connection", "close");
    request->send(response);
  
  },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    //Serial.println("filename = " + filename);
    if(filename != "") {
    if(!index){
      //#ifdef DEBUG
        Serial.printf("start firmware update: %s\n", filename.c_str());
      //#endif
      Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
        //#ifdef DEBUG
          Update.printError(Serial);
        //#endif
      }
    }
    } else {
      //("filename empty, aborting");
//     Update.hasError()=true;
    }
    if(!Update.hasError()){
      if(Update.write(data, len) != len){
        //#ifdef DEBUG
          Serial.println(F("update failed with error: " ));
          Update.printError(Serial);
        //#endif
      }
    }
    if(final){
      //#ifdef DEBUG
      if(Update.end(true)){
        Serial.printf("firmware Update Success: %uB\n", index+len);
      } else {
        Update.printError(Serial);
      }
      //#endif
    }
  });
// ***********************************************************************************
//                                     homepage
// ***********************************************************************************
server.on("/SW=BACK", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    request->redirect( String(requestUrl) );
});

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//    loginBoth(request, "both");
    request->send_P(200, "text/html", P1_HOMEPAGE );
});

server.on("/STYLESHEET_HOME", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", STYLESHEET_HOME);
});
server.on("/STYLESHEET_SUBS", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", STYLESHEET_SUBS);
});
server.on("/JAVASCRIPT", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", JAVA_SCRIPT);
});
server.on("/SECURITY", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/css", SECURITY);
});

server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    //Serial.println("favicon requested");
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", FAVICON, FAVICON_len);
    request->send(response);
});

server.on("/MENU", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    request->send_P(200, "text/html", MENUPAGE);
});

server.on("/DENIED", HTTP_GET, [](AsyncWebServerRequest *request) {
   request->send_P(200, "text/html", REQUEST_DENIED);
});
// ***********************************************************************************
//                                   basisconfig
// ***********************************************************************************
server.on("/BASISCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    strcpy( requestUrl, request->url().c_str() );// remember this to come back after reboot
    zendPageBasis(request);
    //request->send(200, "text/html", toSend);
});

server.on("/basisconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleBasisconfig(request);
    //request->send(200, "text/html", toSend);
    request->redirect( String(requestUrl) );
});

server.on("/MQTT", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    strcpy( requestUrl, request->url().c_str() );
    zendPageMQTTconfig(request);
});

server.on("/MQTTconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
handleMQTTconfig(request);
request->redirect( String(requestUrl) );
});

server.on("/GEOCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    strcpy( requestUrl, request->url().c_str() );
    zendPageGEOconfig(request);
});

server.on("/geoconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
    //DebugPrintln(F("geoconfig requested"));
    handleGEOconfig(request);
    request->redirect( String(requestUrl) );
});

server.on("/REBOOT", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    actionFlag = 10;
    confirm(); 
    request->send(200, "text/html", toSend);
});

server.on("/STARTAP", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    //DebugPrintln("We gaan de gegevens wissen");
    String toSend = F("<!DOCTYPE html><html><head><script type='text/javascript'>setTimeout(function(){ window.location.href='/SW=BACK'; }, 5000 ); </script>");
    toSend += F("</head><body><center><h2>OK the accesspoint is started.</h2>Wait unil the led goes on.<br><br>Then go to the wifi-settings on your pc/phone/tablet and connect to ESP-");
    toSend += String(ESP.getChipId()) + " !";
    request->send ( 200, "text/html", toSend ); //zend bevestiging
    actionFlag = 11;
});

server.on("/ABOUT", HTTP_GET, [](AsyncWebServerRequest *request) {
    //Serial.println(F("/INFOPAGE requested"));
    loginBoth(request, "both");
    handleAbout(request);
});
server.on("/TEST", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
    loginBoth(request, "admin");
    actionFlag = 44;
    request->send( 200, "text/html", "<center><br><br><h3>checking zigbee.. please wait a minute.<br>Then you can find the result in the log.<br><br><a href=\'/PAGE\'>click here</a></h3>" );
});

server.on("/REPORT", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    strcpy( requestUrl, request->url().c_str() );
    //handleReport(request);
    request->send_P(200, "text/html", REPORTPAGE, putReport);
});

 
// ********************************************************************




// Handle Web Server Events
events.onConnect([](AsyncEventSourceClient *client){
//  if(client->lastId()){
//    Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
//  }
});
server.addHandler(&events);



// ********************************************************************
//                    X H T  R E Q U E S T S
//***********************************************************************


server.on("/get.Data", HTTP_GET, [](AsyncWebServerRequest *request) {
// this link provides the general data on the frontpage
    char temp[13]={0};
    uint8_t remote = 0;
    if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link

// {"ps":"05:27 hr","pe":"21:53 hr","cnt":3,"rm":0,"st":1,"sl":1}  length 62
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    //StaticJsonDocument<160> doc; //(160);
    DynamicJsonDocument root(240); //(160);
    //long ECON_LT = 0; //Meter reading Electrics - consumption low tariff
    //long ECON_HT = 0; //Meter reading Electrics - consumption high tariff
    //long ERET_LT = 0; //Meter reading Electrics - return low tariff
    //long ERET_HT = 0; //Meter reading Electrics - return high tariff
    //long PACTUAL_CONS = 0;  //Meter reading Electrics - Actual consumption
    //long PACTUAL_RET = 0;  //Meter reading Electrics - Actual return
    //long mGAS = 0;  //Meter reading Gas
    //long prevGAS = 0;
    float enReturn = ERET_HT+ERET_LT;
    float enCons   = ECON_HT+ECON_LT;
    //long EnLt = mECLT-mERLT; // saldo
    float Power = PACTUAL_CON - PACTUAL_RET;

    root["ECON_HT"] = round3(ECON_HT);
    //if(ECON_HT != 0 ) root["eNcht"] = round3(ECON_HT); else root["eNcht"] = "n/a";
    root["ECON_LT"] = round3(ECON_LT); 
    //if(ECON_LT != 0 ) root["eNclt"] = round3(ECON_LT); else root["eNclt"] = "n/a";
    
    root["ERET_HT"] = round3(ERET_HT);// else root["eNrht"] = "n/a";
    root["ERET_LT"] = round3(ERET_LT);// else root["eNrlt"] = "n/a";

    root["PACTUAL_CON"] = round0(PACTUAL_CON); //else root["pac"] = "n/a";
    root["PACTUAL_RET"] = round0(PACTUAL_RET); //else root["par"] = "n/a";

    root["enR"] = round3(enReturn); //else root["enR"] = "n/a";
    root["enC"] = round3(enCons); //else root["enC"] = "n/a";
    root["aPo"] = round0(Power); //else root["aPo"] = "n/a";
    root["gAs"] = round3(mGAS); //else root["gAs"] = "n/a";
    root["rm"] = remote;
   
    serializeJson(root, * response);
    request->send(response);
});


// if everything failed we come here
server.onNotFound([](AsyncWebServerRequest *request){
  //Serial.println("unknown request");
  handleNotFound(request);
});

server.begin(); 
}

void confirm() {
toSend="<html><body onload=\"setTimeout(function(){window.location.href='/';}, 3000 );\"><br><br><center><h3>processing<br>your request,<br>please wait</html>";
}
