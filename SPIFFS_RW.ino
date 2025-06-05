// ******************   spiffs lezen  *************************

// als er geen spiffs bestand is dan moet hij eigenlijk altijd een ap openenen
void SPIFFS_read() {
  //DebugPrintln("mounting FS...");
 if (LittleFS.begin()) {
    //DebugPrintln("mounted file system");

       if( file_open_for_read("/logChar.txt") ) {
                Serial.println("\nread logChar.txt\n");
          } else {
             Serial.println("\nlogChar.txt not opened\n");
          }
       if( file_open_for_read("/testFile.txt") ) {
                Serial.println("\nread testFile\n");
                testTelegram = true;
          } else {
             Serial.println("\ntestFile.txt not opened\n");
          }
       if( file_open_for_read("/wificonfig.json") ) {
                Serial.println("\nread wificonfig\n");
          } else {
             Serial.println("wificonfig.json not opened\n");
          }
       
       if( file_open_for_read("/basisconfig.json") ) {     
             Serial.println("\nread basisconfig\n");
          } else {
          Serial.println("basisconfig.json not opened\n");
        } 
       if( file_open_for_read("/mqttconfig.json") ) {     
             Serial.println("\nmqttconfig read");
          } else {
          Serial.println("mqttconfig.json not opened");
        }         
//       if( file_open_for_read("/domconfig.json") ) {     
//             Serial.println("domconfig read");
//          } else {
//          Serial.println("domconfig.json not opened");
//        } 
  
  } 
 // einde spiffs lezen 3 bestanden

}  

// writeStruct(bestand, month); // alles opslaan in SPIFFS

void writeStruct( String whichfile, int mnd) {
      File configFile = LittleFS.open(whichfile, "w");
      // f.i. monthly_vals0.str
        if (!configFile)
           {
             Serial.print(F("Failed open for write : ")); Serial.println(whichfile);            
           } 
           
              Serial.print(F("Opened for write....")); Serial.println(whichfile);
              configFile.write( (unsigned char *)&MVALS[mnd], sizeof(MVALS[mnd]) );
              configFile.close();
           
}

bool readStruct(String whichfile ,int mnd) {
      //input = //mvalues_0.str
      File configFile = LittleFS.open(whichfile, "r");

      Serial.print(F( "readStruct mnd = ")); Serial.println( String(mnd) );  
        if (!configFile)
           {
              Serial.print(F("Failed to open for read")); Serial.println(whichfile);
              return false;           
           } 
              Serial.print(F("reading ")); Serial.println(whichfile);
              configFile.read( (unsigned char *)&MVALS[mnd], sizeof(MVALS[mnd]) );
              configFile.close();
              return true;
 }



// **************************************************************************** 
//             save data in SPIFFS                         *  
// ****************************************************************************

void logCharsave() {
   //DebugPrintln("saving config");
    if( LittleFS.exists("/logChar.txt") ){
      Serial.println("logChar exists, we don't overwrite it");
      return;
    }
    DynamicJsonDocument doc(150);
    JsonObject json = doc.to<JsonObject>();   
    json["content"] = logChar;

    File configFile = LittleFS.open("/logChar.txt", "w");
    serializeJson(json, Serial);
    Serial.println(F("")); 
    serializeJson(json, configFile);
    configFile.close();
}

void testFilesave() {
   //DebugPrintln("saving config");
    if( LittleFS.exists("/testFile.txt") ){
      Serial.println("testfile exists, we don't overwrite it");
      return;
    }
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();   
    json["content"] = teleGram;

    File configFile = LittleFS.open("/testFile.txt", "w");
    serializeJson(json, Serial);
    Serial.println(F("")); 
    serializeJson(json, configFile);
    configFile.close();
}



void wifiConfigsave() {
   //DebugPrintln("saving config");

    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();   
//    json["ip"] = static_ip;
    json["pswd"] = pswd;
    json["longi"] = longi;
    json["lati"] = lati;
    json["gmtOffset"] = gmtOffset;
    json["zomerTijd"] = zomerTijd;
    Serial.println("spiffs save securityLevel = " + String(securityLevel));
    json["securityLevel"] = securityLevel;
    File configFile = LittleFS.open("/wificonfig.json", "w");
    if (!configFile) {
    Serial.println("open basisconfig failed");
    }
    if(diagNose){ 
    serializeJson(json, Serial);
    Serial.println("");     
    } 
    serializeJson(json, configFile);
    configFile.close();
}


void basisConfigsave() {
    //DebugPrintln("saving basis config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();
    json["userPwd"] = userPwd;
    json["meterType"] = meterType;
    json["pollFreq"] = pollFreq;
    json["Polling"] = Polling;
    json["diagNose"] = diagNose;    
    File configFile = LittleFS.open("/basisconfig.json", "w");
    if (!configFile) {
    Serial.println("open basisconfig failed");
    }
    if(diagNose){ 
    serializeJson(json, Serial);
    Serial.println("");     
    }
    serializeJson(json, configFile);
    configFile.close();
    }

void mqttConfigsave() {
   //DebugPrintln("saving mqtt config");
    DynamicJsonDocument doc(1024);
    JsonObject json = doc.to<JsonObject>();

    json["Mqtt_Broker"] = Mqtt_Broker;
    json["Mqtt_Port"] = Mqtt_Port;    
    json["gas_Idx"] = gas_Idx;
    json["el_Idx"] = el_Idx;
    json["Mqtt_outTopic"] = Mqtt_outTopic;
    json["Mqtt_Username"] = Mqtt_Username;
    json["Mqtt_Password"] = Mqtt_Password;
    json["Mqtt_Clientid"] = Mqtt_Clientid;    
    json["Mqtt_Format"] = Mqtt_Format;    
    File configFile = LittleFS.open("/mqttconfig.json", "w");
    if (!configFile) {
    Serial.println("open mqttconfig failed");
    }
    if(diagNose){ 
      serializeJson(json, Serial);
      Serial.println("");     
    }
    serializeJson(json, configFile);
    configFile.close();
}



bool file_open_for_read(String bestand) {
      //DebugPrint("we are in file_open_for_read, bestand = "); //DebugPrintln(bestand); 
      if (!LittleFS.exists(bestand)) return false;
      
      //file exists, reading and loading
      //DebugPrintln("bestand bestaat");
        File configFile = LittleFS.open(bestand, "r");
        if (!configFile) return false;
        Serial.println("opened config file" + bestand);
           size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
           std::unique_ptr<char[]> buf(new char[size]);
           configFile.readBytes(buf.get(), size);
           DynamicJsonDocument doc(1024);
           auto error = deserializeJson(doc, buf.get());
           serializeJson(doc, Serial); Serial.println(F(""));
             if (error) return false; 
              //DebugPrintln("parsed json");
              String jsonStr = ""; // we print the json object to a string
            // now we can check if an entry exists
            // otherwise a crash occurs
                //serializeJson(doc, Serial);
                serializeJson(doc, jsonStr);

            if (bestand == "/logChar.txt") {
                      if(jsonStr.indexOf("content") > 0){ strcpy(logChar, doc["content"]);}
                      Serial.println("\n\nspiffs logChar = " + String(logChar));          
            }
            
            if (bestand == "/testFile.txt") {
                      if(jsonStr.indexOf("content") > 0){ strcpy(teleGram, doc["content"]);}
                      Serial.println("\n\nspiffs testFile = " + String(teleGram));          
            }
            
            if (bestand == "/wificonfig.json") {
                      //if(jsonStr.indexOf("ip") > 0){ strcpy(static_ip, doc["ip"]);}
                      if(jsonStr.indexOf("pswd") > 0){ strcpy(pswd, doc["pswd"]);}
                      if(jsonStr.indexOf("longi") > 0){longi = doc["longi"].as<float>();}
                      if(jsonStr.indexOf("lati") > 0){lati = doc["lati"].as<float>();}                      
                      if(jsonStr.indexOf("gmtOffset") > 0){ strcpy(gmtOffset, doc["gmtOffset"]);}
                      if(jsonStr.indexOf("zomerTijd") > 0){zomerTijd = doc["zomerTijd"].as<bool>();}
                      if(jsonStr.indexOf("securityLevel") > 0){securityLevel = doc["securityLevel"].as<int>();}
                      //Serial.println("spiffs securityLevel = " + String(securityLevel));
            }

            if (bestand == "/basisconfig.json") {
                     if(jsonStr.indexOf("userPwd") > 0) { strcpy (userPwd, doc["userPwd"] );}
                     //if(jsonStr.indexOf("dom_Address")   >  0 ) { strcpy(dom_Address,   doc["dom_Address"])         ;}
                     //if(jsonStr.indexOf("dom_Port") >  0 ) { dom_Port = doc["dom_Port"].as<int>() ;} 
                     if(jsonStr.indexOf("meterType")>  0 ) { meterType = doc["meterType"].as<int>() ;}
                     if(jsonStr.indexOf("pollFreq")>  0 ) { pollFreq = doc["pollFreq"].as<int>() ;}
                     if(jsonStr.indexOf("Polling") > 0) {Polling = doc["Polling"].as<bool>();}
                     if(jsonStr.indexOf("diagNose") > 0) {diagNose = doc["diagNose"].as<bool>();}
              }            

            if (bestand == "/mqttconfig.json"){
                     if(jsonStr.indexOf("Mqtt_Broker")   >  0 ) { strcpy(Mqtt_Broker,   doc["Mqtt_Broker"])         ;}
                     if(jsonStr.indexOf("Mqtt_Port")     >  0 ) { Mqtt_Port =           doc["Mqtt_Port"].as<int>()  ;}
                     if(jsonStr.indexOf("Mqtt_outTopic") >  0 ) { strcpy(Mqtt_outTopic, doc["Mqtt_outTopic"])       ;}         
                     if(jsonStr.indexOf("Mqtt_Username") >  0 ) { strcpy(Mqtt_Username, doc["Mqtt_Username"])       ;}
                     if(jsonStr.indexOf("Mqtt_Password") >  0 ) { strcpy(Mqtt_Password, doc["Mqtt_Password"])       ;}
                     if(jsonStr.indexOf("Mqtt_Clientid") >  0 ) { strcpy(Mqtt_Clientid, doc["Mqtt_Clientid"])       ;}
                     if(jsonStr.indexOf("Mqtt_Format")   >  0 ) { Mqtt_Format =         doc["Mqtt_Format"].as<int>();}
                     if(jsonStr.indexOf("gas_Idx")  >  0 )      { gas_Idx =             doc["gas_Idx"].as<int>() ;}         
                     if(jsonStr.indexOf("el_Idx")   >  0 )      { el_Idx =              doc["el_Idx"].as<int>() ;}
            }

              return true;
 }

// we do this before swap_to_zigbee
//void printStruct( String bestand ) {
////input String bestand = "/Inv_Prop" + String(x) + ".str";
//      //String bestand = bestand + String(i) + ".str"
//      //leesStruct(bestand); is done at boottime
////      #ifdef DEBUG
////      int ivn = bestand.substring(9,10).toInt();
////      Serial.println("Inv_Prop[" + String(ivn) + "].invLocation = " + String(Inv_Prop[ivn].invLocation));     
////      Serial.println("Inv_Prop[" + String(ivn) + "].invSerial = " + String(Inv_Prop[ivn].invSerial));
////      Serial.println("Inv_Prop[" + String(ivn) + "].invID = " + String(Inv_Prop[ivn].invID)); 
////      Serial.println("Inv_Prop[" + String(ivn) + "].invType = " + String(Inv_Prop[ivn].invType));
////      Serial.println("Inv_Prop[" + String(ivn) + "].invIdx = " + String(Inv_Prop[ivn].invIdx));
////      Serial.println("Inv_Prop[" + String(ivn) + "].conPanels = " + String(Inv_Prop[ivn].conPanels[0])  + String(Inv_Prop[ivn].conPanels[1]) + String(Inv_Prop[ivn].conPanels[2]) + String(Inv_Prop[ivn].conPanels[3]));      
////      Serial.println("");
////      Serial.println("****************************************");
////      #endif
//}
