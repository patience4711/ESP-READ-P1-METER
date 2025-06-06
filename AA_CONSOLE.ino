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
  <b>DELETE-FILE=/filename: </b> delete a file.<br><br>
  <b>POLL-METER: </b> poll the p1 meter now <br><br>
  <b>TEST-MOSQUITTO: </b>sends a mqtt testmessage<br><br>
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
            


 // ************  test mosquitto *******************************          
           if (strcasecmp(txBuffer,"TEST-MOSQUITTO") == 0) {  
              ws.textAll("test mosquitto");
              actionFlag=49; // perform the healthcheck
              diagNose=true;
              return;             
          } else 

          // see files
           if (strcasecmp(txBuffer,"PRINTOUT-SPIFFS") == 0) {  
              //we do this in the loop
              actionFlag = 46;
              return;             
          
          } else 

           if (strcasecmp(txBuffer,"CLEAR-CONSOLE") == 0) {  
              ws.textAll("clearWindow");
              return;             
          } else           
          
           if (strncasecmp(txBuffer,"DELETE-FILE=",12) == 0) {  
              //input can be DELETE-FILE=filename
              ws.textAll("len = " + String(len));
              String bestand="";
              for(int i=12;  i<len+1; i++) { bestand += String(txBuffer[i]); }
               ws.textAll("bestand = " + bestand); 
              if (SPIFFS.exists(bestand)) 
              {
                  ws.textAll("going to delete file " + bestand); 
                  {
                      SPIFFS.remove(bestand);
                      ws.textAll("file " + bestand + " removed!"); 
                  }
              
              } else 
              { 
                 ws.textAll("no such file, forgot the / ??");
              }
              return;                      
 
          } else

      if (strcasecmp(txBuffer, "POLL-METER") == 0) 
      {
          actionFlag = 26;
          ws.textAll("going to poll the meter") ;  
     
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
