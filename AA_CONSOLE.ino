//<link rel="icon" type="image/x-icon" href="/favicon.ico" />

const char CONSOLE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<title>ESP-P1 METER</title>
<meta name="viewport" content="width=device-width, initial-scale=1">

<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<script>
function helpfunctie() {
document.getElementById("help").style.display = "block";
}
function sl() {  
document.getElementById("help").style.display = "none";
}
</script>

<style>
 tr {height:16px !important;
 font-size:15px !important;
 } 
 li a:hover {
   background-color: #333 !important;
}
#help {
  background-color: #ffffff; 
  border: solid 2px; 
  display:none; 
  padding:4px;
  width:94vw;
}
</style>
</head>
<body>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>CONSOLE COMMANDS</h3>
  <b>PRINTOUT-SPIFFS: </b> show filesystem<br><br>
  <b>DELETE-FILE=filename: </b> delete a file.<br><br>
  <b>TEST-Serial: </b> Serial loopback test.<br><br>
  <b>DELETE-TESTFILES: </b> delete the testfiles<br><br>
  <b>POLL-METER: </b> poll the p1 meter now <br><br>
  <b>TEST-MOSQUITTO: </b>sends a mqtt testmessage<br><br>
  <b>FORCE-VALUES: </b>set some values to test with<br><br>     
  <b>WRITE-MONTH: </b> write values of this month to file<br><br>
  <b>DECODE-TEST: </b>decode the telegram in testfile<br><br>
  <b>SHOW-STARTLOG: </b>print the startlog<br><br>   
  <b>CLEAR-CONSOLE: </b> clear console window<br><br> 
  </div>

<div id='msect'>
<div id='menu'>
<a href='/MENU' onclick='confirmExit()' class='close'>&times;</span></a>
<a href='#' onclick='helpfunctie()'>help</a>
<a href='#'><input type="text" placeholder="type here" id="tiep"></a>
</div>
</div>  
<br>  
<div id='msect'>
<br>  
<div id='msect'>
  <div class='divstijl' style='height:84vh; border:1px solid; padding-left:10px;'>
  <table id='tekstveld'></table>
  </div>
 </div>

<script>
  var field = document.getElementById('tekstveld');
  //var gateway = `ws://${window.location.hostname}/ws`;
  var gateway = `${(window.location.protocol == "https:"?"wss":"ws")}://${window.location.hostname}/ws`;
  var websocket;
  var inputField = document.getElementById('tiep');

  window.onbeforeunload = confirmExit;
  function confirmExit()
  {
      alert("close the console?");
      ws:close();  
  }  
  
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    field.insertAdjacentHTML('beforeend', "<tr><td>* * connection opened * *");
    inputField.focus();
    }
  function onClose(event) {
    console.log('Connection closed');
    field.insertAdjacentHTML('beforeend', "<tr><td>* * connection closed * *");
    //setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    //var message = event.data;
    field.insertAdjacentHTML('beforeend', "<tr><td>" + event.data );
    if (field.rows.length > 20) {
    var rtm = field.rows.length - 20;
    for (let x=0; x<rtm; x++) { field.deleteRow(0); }
  }
    if (event.data == "clearWindow") { 
    for (let i = 0; i < 22; i++) {
        field.deleteRow(0); }
    }
   }
 
  function onLoad(event) {
    initWebSocket();
    sendEvent();
  }

  function sendEvent() {
    inputField.addEventListener('keyup', function(happen) {
    if (happen.keyCode === 13) {
       happen.preventDefault();
       sendData();
       }   
    });
  }  
  function sendData(){
  var data = inputField.value; 
  websocket.send(data, 1);
  inputField.value = "";
  }

function disConnect() {
  alert("close the console");
  ws:close();
  window.location.href='/MENU';   
}
</script>
</body>
</html>
)=====";


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  diagNose = true;
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  for(int i=0; i<len; i++ ) 
  {
  txBuffer[i] = data[i];
  }
  txBuffer[len]='\0'; // terminate the array

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
  {
      data[len] = 0;
            
//           if (strncasecmp(txBuffer+3,"HEALTH",6) == 0) {  
//              ws.textAll("check zb system");
//              actionFlag=44; // perform the healthcheck
//              diagNose=true;
//              return;             
//          } else          

 // ************  test mosquitto *******************************          
           if (strncasecmp(txBuffer+3,"TESTMQTT",8) == 0) {  
              ws.textAll("test mosquitto");
              actionFlag=49; // perform the healthcheck
              diagNose=true;
              return;             
          } else 

          // see files
           if (strncasecmp(txBuffer+3,"FILES",5) == 0) {  
              //we do this in the loop
              actionFlag = 46;
              return;             
          
          } else 
            if (strncasecmp(txBuffer+3,"TESTDEL",7) == 0) {  
              ws.textAll("going to delete testfiles ");
              if (SPIFFS.exists("/testFile.txt")) SPIFFS.remove("/testFile.txt");
              actionFlag = 46; // show the existing files
              return;             
          
          } else            
          
           if (strncasecmp(txBuffer+3,"DELETE=",7) == 0) {  
              //input can be 10;DELETE=filename
              String bestand="";
              for(int i=10;  i<len+1; i++) { bestand += String(txBuffer[i]); }
               ws.textAll("bestand = " + bestand); 
              if (SPIFFS.exists(bestand)) 
              {
                  ws.textAll("going to delete file " + bestand); 
                  if(bestand.indexOf("Inv_Prop") == -1 ) 
                  {
                      SPIFFS.remove(bestand);
                      ws.textAll("file " + bestand + " removed!"); 
                  } else {
                      ws.textAll("inverterfile not removed, use 10;erase!"); 
                  }
              
              } else 
              { 
                 ws.textAll("no such file");
              }
              return;                      
 
          } else

      if (strncasecmp(txBuffer+3, "FORCE",5) == 0) // normal operation
      {
//long ECON_LT = 0; //Meter reading Electrics - consumption low tariff
//long ECON_HT = 0; //Meter reading Electrics - consumption high tariff
//long ERET_LT = 0; //Meter reading Electrics - return low tariff
//long ERET_HT = 0; //Meter reading Electrics - return high tariff
//long PACTUAL_CON = 0;  //Meter reading Electrics - Actual consumption
//long PACTUAL_RET = 0;  //Meter reading Electrics - Actual return
//long mGAS = 0;  //Meter reading Gas
//long prevGAS = 0;
          
          ECON_LT = 51.111; //kwh
          ECON_HT = 11.222;
          ERET_LT = 26.333;
          ERET_HT = 17.444;
          PACTUAL_CON = 345.000;
          PACTUAL_RET = 565.000;
          mGAS = 17.713;
          ws.textAll("values forced") ;  
     
      } else 

      if (strncasecmp(txBuffer+3, "POLL",4) == 0) 
      {
          actionFlag = 26;
          ws.textAll("going to poll the meter") ;  
     
      } else      
      
//      if (strncasecmp(txBuffer+3, "DECODE",6) == 0) 
//      // decode the current telegram
//      {
//          actionFlag = 28;
//          ws.textAll("going to decode the telegram") ;  
//      } else  
      
      if (strncasecmp(txBuffer+3, "STARTLOG",8) == 0) 
      //show the testresults at boot
      {
          actionFlag = 27;
          ws.textAll("going to show the debugfiles") ;  
      } else 
      
     
      if (strncasecmp(txBuffer+3, "MONTH",5) == 0) 
      {
          ws.textAll("write the data into current month") ;
          writeMonth(month() );  
          //ws.textAll("MVALS[8].EC_LT = " + String(MVALS[8].EC_LT)); 
          //ws.textAll("MVALS[8].ER_HT = " + String(MVALS[8].ER_HT));
 
     
      } else {

       
       ws.textAll("unknown command"); 
      }
  
  }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    //Serial.println("onEvent triggered");
    switch (type) {
      case WS_EVT_CONNECT:
        //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        //Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        //Serial.println("WebSocket received data");
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
