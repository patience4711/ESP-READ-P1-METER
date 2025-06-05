// ************************************************************************************
// *                        START wifi
// ************************************************************************************
void start_wifi() {
 WiFi.softAPdisconnect(true);
 WiFi.mode(WIFI_STA);
 WiFi.hostname("ESP-P1METER");

 WiFi.mode(WIFI_STA); // geen ap op dit moment 

// we gaan 10 pogingen doen om te verbinden
// met de laatst gebruikte credentials
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     //Serial.print("*");
     event+=1;
     if (event==10) {break;}
  }
// als het verbinden is mislukt gaan we naar het configportal  
  if (event>9) {
     event=0;

     start_portal(); // stel het portal in
     }
//Serial.print(F("# connection attempts = "));  //Serial.println(event);
event=0; // we kunnen door naar de rest
//checkFixed();
  
  start_asyserver();
}
//// *************************************************************************
////                      SERVER STARTEN




void loginBoth(AsyncWebServerRequest *request, String who) {
  //String authFailResponse = "<h2>login failed <a href='/'>click here</a></h2>";
  if (who == "admin" ){
  const char* www_realm = "login as administrator."; 
    if (!request->authenticate("admin", pswd)) return request->requestAuthentication();
  }
  if (who == "both" ){
  const char* www_realm = "login as administrator or user."; 
    if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPwd)) return request->requestAuthentication();
  }
}
