const char MQTTCONFIG[] PROGMEM = R"=====(
<body>
<script type='text/javascript'>
function helpfunctie() {
document.getElementById("help").style.display = "block";
}
function sl() {  
document.getElementById("help").style.display = "none";
}
</script>

<div id='msect'>
<div id='bo'></div>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span>
  <b>mosquitto message format:</b><br>
  <b>f0:</b> mosquitto disabled<br><br>  
  <b>f1:</b> {"idx":123,"nvalue":0,"svalue":"123.22;234.55;123.22;222.33"}<br><br>
  <b>f2:</b> {"econ_lt":123.22,"econ_ht":234.55,"eret_ht":123.22,"eret_lt":222.33,"gas":1234.555}<br><br>
  <b>f3:</b> {"field1=:123.22&field2=234.55&field3=123.22&field4=222.33&field5=1234.555&status=MQTTPUBLISH")<br>
  <br>Format 2 has the retainflag set.
  <br><br> 
  
  <b>receive topic:</b><br>The topic where will be subscribed for incoming messages.<br><br>
  <b>mqtt_username en password</b><br>Optional, these can be left empty.
  <br><br>

  </div>
</div>

<div id='msect'><div id='bo'></div>
<ul> <li><a href='#' onclick='helpfunctie()'>help</a>
<li id='fright'><span class='close' onclick='cl();'>&times;</span>
<li id='sub'><a href='#' onclick='submitFunction("/SW=BACK")'>save</a></li></ul></div>
<div id='msect'><kop>MOSQUITTO CONFIGURATION</kop></div>
<div id='msect'>
  <div class='divstijl'>
  <center>
  <form id='formulier' method='get' action='MQTTconfig' oninput='showSubmit();'><table>
  
  
   <tr><td>format:&nbsp<td><select name='fm' class='sb1' id='sel' >
    <option value='0' fm_0>disabled</option>
    <option value='1' fm_1>format 1</option>
    <option value='2' fm_2>format 2</option>
    <option value='3' fm_3>format 3</option>
    </select>
  <tr><td >address<td><input class='inp6' name='mqtAdres' value='{mqttAdres}' size='31' placeholder='broker adres'></tr>
  <tr><td >port<td><input class='inp2' name='mqtPort' value='{mqttPort}' size='31' placeholder='mqtt port'></tr>
  
  <tr><td>send topic:&nbsp<td><input class='inp6' name='mqtoutTopic' value='{mqttoutTopic}' placeholder='mqtt topic receive' length='60'></tr>
  <tr><td>state idx:&nbsp<td><input class='inp2' name='mqidx' value='{idx}' size='4' length='4'></tr>
  <tr><td>username:&nbsp<td><input class='inp6' name='mqtUser' value='{mqtu}'></td></tr>
  <tr><td>password:&nbsp<td><input class='inp6' name='mqtPas' value='{mqtp}'></td></tr>
  <tr><td>client id:&nbsp<td><input class='inp6' name='mqtCi' value='{mqtc}'></td></tr>
  </form>
  </td></table>
  </div><br>
</div>

</body></html>
)=====";
  //<li><a href='/MQTT_TEST' >test</a></ul>

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
//webPage.replace("{mqttinTopic}",  String(Mqtt_inTopic)  );
webPage.replace("{mqttoutTopic}", String(Mqtt_outTopic) );
webPage.replace("{mqtu}",         String(Mqtt_Username) );
webPage.replace("{mqtp}",         String(Mqtt_Password) );
webPage.replace("{idx}"          , String(Mqtt_stateIDX) ); 
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

void handleMQTTconfig(AsyncWebServerRequest *request) {
  //collect serverarguments
  strcpy( Mqtt_Broker  , request->getParam("mqtAdres")   ->value().c_str() );
  strcpy( Mqtt_Port    , request->getParam("mqtPort")    ->value().c_str() );
  strcpy( Mqtt_outTopic, request->getParam("mqtoutTopic")->value().c_str() );
  strcpy( Mqtt_Username, request->getParam("mqtUser")    ->value().c_str() );
  strcpy( Mqtt_Password, request->getParam("mqtPas")     ->value().c_str() );
  strcpy( Mqtt_Clientid, request->getParam("mqtCi")     ->value().c_str() );  
  Mqtt_stateIDX = request->arg("mqidx").toInt(); //values are 0 1 2
  Mqtt_Format = request->arg("fm").toInt(); //values are 0 1 2 3 4 5

  //DebugPrintln("saved mqttconfig");
  mqttConfigsave();  // 
  actionFlag=24; // reconnect with these settings
  
}
