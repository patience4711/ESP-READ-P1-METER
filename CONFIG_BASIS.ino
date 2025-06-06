//{ check

//    webPage.replace("{domAdres}",    String(dom_Address)   );
//    webPage.replace("{domPort}",     String(dom_Port)     );
//<tr><td >domoticz ip<td><input class='inp6' name='domAdres' value='{domAdres}' size='31' placeholder='broker adres'></tr>
//  <tr><td >domoticz port<td><input class='inp2' name='domPort' value='{domPort}' size='31' placeholder='dom port'></tr>

const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
<div id="menu">

<a href="#" id="sub" onclick='submitFunction()'>save</a>
<a href="#" class='close' onclick='cl();'>&times;</a>
</div>
</div>

<div id='msect'><kop>P1-METER SETTINGS</kop></div>

<div id='msect'>
  <div class='divstijl' style='width: 480px; height:56vh;'>
  <form id='formulier' method='get' action='submitform' oninput='showSubmit()'>
  <center><table>
    <tr><td>user passwd<td><input  class='inp5' name='pw1' length='11' placeholder='max. 10 char' value='{pw1}' pattern='.{4,10}' title='between 4 en 10 characters'></input> 
  </td></tr>
  <tr><td class="cap">meter type<td><select name='mtype' class='sb1' id='sel' >
    <option value='0' mtype_0>NO METER</option>
    <option value='1' mtype_1>SAGEMCOM XS210_D ESMR5</option>
    <option value='2' mtype_2>LANDIS GYR E350 ZMF100</option></select>
    </tr>  
  <tr><td class="cap">polling frequency<td><select name='pfreq' class='sb1' id='sel2' >
    <option value='30' pfreq_0>every 30 sec</option>
    <option value='60' pfreq_1>every 1 min</option>
    <option value='300' pfreq_2>every 5 min</select>
    </tr>
    
    <tr><td>serial debug:<td><input type='checkbox' style='width:30px; height:30px;' name='debug' #sjek ></input></td></tr>
    <tr><td>auto polling:<td><input type='checkbox' style='width:30px; height:30px;' name='pL' #check></input></td><tr>
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

     switch (pollFreq) { 
       case 30:
          webPage.replace("pfreq_0", "selected");
          break;
       case 60:
          webPage.replace("pfreq_1", "selected");
          break;
       case 300:
          webPage.replace("pfreq_2", "selected");
          break;
    }   
    
    if (Polling) { webPage.replace("#check", "checked");   }
    if (diagNose){ webPage.replace("#sjek","checked"); }
    request->send(200, "text/html", webPage);
    webPage=""; // free up
}


//void handleBasisconfig(AsyncWebServerRequest *request) { // form action = handleConfigsave
//// verzamelen van de serverargumenten   
//  strcpy(userPwd, request->arg("pw1").c_str());
//  strcpy( dom_Address  , request->getParam("domAdres")  ->value().c_str() );
//  dom_Port    =          request->arg("domPort").toInt();
//  gas_Idx     =          request->arg("gasidx").toInt();
//  el_Idx      =          request->arg("elidx").toInt();
//  meterType   =          request->arg("mtype").toInt(); //values are 0 1 2 
//  //BEWARE CHECKBOX
//  String dag = "";
//  if(request->hasParam("pL")) {
//  dag = request->getParam("pL")->value();  
//  }
//  if (dag == "on") { Polling = true; } else { Polling = false;}  
//  basisConfigsave();  // alles opslaan
// 
//
//  //if(USB_serial) Serial.println("basisconfig saved");
//  actionFlag=25; // recalculates the time with these new values 
//}
