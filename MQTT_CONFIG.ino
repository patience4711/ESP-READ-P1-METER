//{ check
const char MQTTCONFIG[] PROGMEM = R"=====(
<body>
<script>
function sluit() {  
document.getElementById("help").style.display = "none";
}
</script>
<div id='msect'>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span>
  <b>mosquitto message format:</b><br>
  <b>f0:</b> mosquitto disabled<br><br>  
  <b>f1:</b> {"idx":123,"nvalue":0,"svalue":"ECON_LT,ECON_HT,ERET_LT,ERET_HT,PACTUAL_CON,PACTUAL_RET);"}<br>
             {"idx":915,"nvalue":0,"svalue":"GAS;"}<br><br>
  
  <b>f2:</b> {"econ_lt":123.22,"econ_ht":234.55,"eret_ht":123.22,"eret_lt":222.33,"gas":1234.555}<br><br>
  <b>f3:</b> {"field1=:ECON_LT&field2=ECON_HT&field3=ERET_LT&field4=ERET_HT&field5=PACTUAL_CON&field6=PACTUAL_RET&field7=GAS&status=MQTTPUBLISH"}<br>
  <br>Format 2 has the retainflag set.
  <br><br> 
  
  <b>receive topic:</b><br>Fixed value, the topic on which is subscribed for incoming mqtt.<br><br>
  <b>mqtt_username en password</b><br>Optional, these can be left empty.<br><br>
  <b>explation</b><br>
  ECON_LT: ECON-HT: energy consumed low/high tariff<br>
  ERET_LT: ERET-HT: energy returned low/high tariff<br>
  PACTUAL_CON: PACTUAL_RET: actual power consume/return<br>

  </div>
</div>
<div id='msect'><div id='menu' style="align-items:left;">
<a href='#' onclick='helpfunctie()'>&quest;</a>
<a href="#" class='close' onclick='cl();'>&times;</a>
<a href="#" id="sub" onclick='submitFunction()'>save</a>
</div></div>
<div id='msect'><kop>P1 METER MOSQUITTO</kop></div>
<div id='msect'>
  <div class='divstijl'>
  <center>
  <form id='formulier' method='get' action='submitform' oninput='showSubmit();'><table>
  
  
   <tr><td>format:&nbsp<td><select name='fm' class='sb1' id='sel' >
    <option value='0' fm_0>disabled</option>
    <option value='1' fm_1>format 1</option>
    <option value='2' fm_2>format 2</option>
    <option value='3' fm_3>format 3</option>
    </select>
  <tr><td >address<td><input class='inp6' name='mqtAdres' value='{mqttAdres}' size='31' placeholder='broker adres'></tr>
  <tr><td >port<td><input class='inp2' name='mqtPort' value='{mqttPort}' size='31' placeholder='mqtt port'></tr>
  
  <tr><td>send topic:&nbsp<td><input class='inp6' name='mqtoutTopic' value='{mqttoutTopic}' placeholder='mqtt topic receive' length='60'></tr>
  <tr><td>receive topic:&nbsp<td><input class='inp6' value='{mqttinTopic}' length='60' tittle='readonly' readonly ></tr>
  <tr><td>gas idx:&nbsp<td><input class='inp2' name='gasidx' value='{domg}'></td></tr>
  <tr><td>el. idx:&nbsp<td><input class='inp2' name='elidx' value='{dome}'></td></tr>
  <tr><td>username:&nbsp<td><input class='inp6' name='mqtUser' value='{mqtu}'></td></tr>
  <tr><td>password:&nbsp<td><input class='inp6' name='mqtPas' value='{mqtp}'></td></tr>
  <tr><td>client id:&nbsp<td><input class='inp6' name='mqtCi' value='{mqtc}'></td></tr>
  </form>
  </td></table>
  </div><br>
</div>

</body></html>
)=====";

void zendPageMQTTconfig(AsyncWebServerRequest *request) {
//DebugPrintln("we are at zendPageMQTTconfig");
//webPage = FPSTR(HTML_HEAD);
//webPage.replace("tieTel", swname );
String webPage;
webPage = FPSTR(HTML_HEAD);
webPage += FPSTR(MQTTCONFIG);  

  //altijd de mqtt gegevens terugzetten
 
webPage.replace("{mqttAdres}",    String(Mqtt_Broker)   );
webPage.replace("{mqttPort}",     String(Mqtt_Port)     );
String intopic = "ESP-P1METER-" + String(ESP.getChipId()) + "/in";
webPage.replace("{mqttinTopic}",  intopic  );
webPage.replace("{mqttoutTopic}", String(Mqtt_outTopic) );
webPage.replace("{domg}",         String(gas_Idx) );
webPage.replace("{dome}",         String(el_Idx) ); 
webPage.replace("{mqtu}",         String(Mqtt_Username) );
webPage.replace("{mqtp}",         String(Mqtt_Password) );
webPage.replace("{mqtc}"         , String(Mqtt_Clientid) );
switch (Mqtt_Format) {
 case 0:
    webPage.replace("fm_0", "selected");
    break;
 case 1:
    webPage.replace("fm_1", "selected");
    break;
 case 2:
    webPage.replace("fm_2", "selected");
    break;
 case 3:
    webPage.replace("fm_3", "selected");
    break;
// case 4:
//    webPage.replace("fm_4", "selected");
//    break;
// case 5:
//    webPage.replace("fm_5", "selected");
//    break;
  }
    request->send(200, "text/html", webPage);
    webPage="";
}

//void handleMQTTconfig(AsyncWebServerRequest *request) {
//  //collect serverarguments
//  strcpy( Mqtt_Broker  , request->getParam("mqtAdres")   ->value().c_str() );
//  //strcpy( Mqtt_Port    , request->getParam("mqtPort")    ->value().c_str() );
//  Mqtt_Port = request->arg("mqtPort").toInt();
//  strcpy( Mqtt_outTopic, request->getParam("mqtoutTopic")->value().c_str() );
//  strcpy( Mqtt_Username, request->getParam("mqtUser")    ->value().c_str() );
//  strcpy( Mqtt_Password, request->getParam("mqtPas")     ->value().c_str() );
//  strcpy( Mqtt_Clientid, request->getParam("mqtCi")     ->value().c_str() );  
//  Mqtt_stateIDX = request->arg("mqidx").toInt(); //values are 0 1 2
//  Mqtt_Format = request->arg("fm").toInt(); //values are 0 1 2 3 4 5
//
//  //DebugPrintln("saved mqttconfig");
//  mqttConfigsave();  // 
//  actionFlag=24; // reconnect with these settings  
//}

//} check
