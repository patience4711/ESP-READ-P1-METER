const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
  <div id='bo'></div>
    <ul><li id='fright'><span class='close' onclick='cl();'>&times;</span>
    <li id='sub'><a href='#' onclick='submitFunction("/SW=BACK")'>save</a></li></ul><br></div>

<div id='msect'>
<kop>P1-METER SETTINGS</kop>
</div>

<div id='msect'>
  <div class='divstijl' style='width: 480px; height:56vh;'>
  <form id='formulier' method='get' action='basisconfig' oninput='showSubmit()'>
  <center><table>
    <tr><td>user passwd<td><input  class='inp5' name='pw1' length='11' placeholder='max. 10 char' value='{pw1}' pattern='.{4,10}' title='between 4 en 10 characters'></input> 
  </td></tr>
  <tr><td >domoticz ip<td><input class='inp6' name='domAdres' value='{domAdres}' size='31' placeholder='broker adres'></tr>
  <tr><td >domoticz port<td><input class='inp2' name='domPort' value='{domPort}' size='31' placeholder='dom port'></tr>
  <tr><td>gas idx:&nbsp<td><input class='inp2' name='gasidx' value='{domg}'></td></tr>
  <tr><td>el. idx:&nbsp<td><input class='inp2' name='elidx' value='{dome}'></td></tr>
  <tr><td class="cap">meter type<td><select name='mtype' class='sb1' id='sel' >
    <option value='0' mtype_0>NO METER</option>
    <option value='1' mtype_1>SAGEMCOM T210_D ESMR5</option>
    <option value='2' mtype_2>LANDIS GYR E350 ZMF100</option></select>
    </tr>  
    <tr><td>auto polling<td><input type='checkbox' style='width:30px; height:30px;' name='pL' #check></input></td><tr>
    </table></form>
  </table>
  </div><br>
</div>
</body></html>
)=====";

void zendPageBasis(AsyncWebServerRequest *request) {
  String(webPage)="";
    //if(USB_serial) Serial.println("zendPageBasis");
    webPage = FPSTR(HTML_HEAD);
    webPage += FPSTR(BASISCONFIG);
    
    // replace data
    //if(USB_serial) Serial.println("dom_Port= "+ String(dom_Port));
    webPage.replace( "'{pw1}'" , "'" + String(userPwd) + "'") ;
    webPage.replace("{domAdres}",    String(dom_Address)   );
    webPage.replace("{domPort}",     String(dom_Port)     );
    webPage.replace("{domg}",         String(gas_Idx) );
    webPage.replace("{dome}",         String(el_Idx) );
    // terugzetten select
    //if(USB_serial) Serial.println("meterType= "+ String(meterType));
    switch (meterType) { 
       case 0:
          webPage.replace("mtype_0", "selected");
          break;
       case 1:
          webPage.replace("mtype_1", "selected");
          break;
       case 2:
          webPage.replace("mtype_2", "selected");
          break;
    }       
    if (Polling) { 
      webPage.replace("#check", "checked");
    }
    request->send(200, "text/html", webPage);
    webPage=""; // free up
}


void handleBasisconfig(AsyncWebServerRequest *request) { // form action = handleConfigsave
// verzamelen van de serverargumenten   
  strcpy(userPwd, request->arg("pw1").c_str());
  strcpy( dom_Address  , request->getParam("domAdres")  ->value().c_str() );
  dom_Port    =          request->arg("domPort").toInt();
  gas_Idx     =          request->arg("gasidx").toInt();
  el_Idx      =          request->arg("elidx").toInt();
  meterType   =          request->arg("mtype").toInt(); //values are 0 1 2 
  //BEWARE CHECKBOX
  String dag = "";
  if(request->hasParam("pL")) {
  dag = request->getParam("pL")->value();  
  }
  if (dag == "on") { Polling = true; } else { Polling = false;}  
  basisConfigsave();  // alles opslaan
 

  //if(USB_serial) Serial.println("basisconfig saved");
  actionFlag=25; // recalculates the time with these new values 
}
