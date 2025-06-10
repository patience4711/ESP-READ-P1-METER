
//<input type="number" id="invCount" value="%count%" hidden>
//<script type="text/javascript" src="SECURITY"></script>
// <li><a href='/LOGPAGE'>log</a></li>
  //<li><a href='/INFOPAGE'>info</a></li>  

//  <li><a href='/ABOUT'>about</a></li>
//  <li><a href='/LOGPAGE'>journal</a></li>
  
//  <li id="ml" style='float:right; display:none'><a id='haha' href='/MENU'>menu</a></li>
//<link rel="icon" type="image/x-icon" href="/favicon.ico" />
const char P1_HOMEPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>

<meta charset='utf-8' name="viewport" content="width=device-width, initial-scale=1">
<title>ESP-P1 METER</title>
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">  


<style>
body {
 background-color: #EEE;
}
span {
  padding: 6px;
}
table, th, td {
  border: 2px solid blue; font-size:16px; padding:6px; text-align:center; border-collapse:collapse;backgound-color:#dfff80;
  }
tr {background-color:#ccffcc;}
//td { width:70px; }

#td0 {width:130px;}
#td1 {width:110px;}
#td2 {width:40px;}
#td3 {width:70px;}


.btn {
  background-color: #199319;
  color: white;
  padding: 5px 22px;
  border-radius:6px;
}

.btn:hover {background: #eeeF; color:black;}

/* Popup container - can be anything you want */
.popup {
  position: relative;
  display: inline-block;
  cursor: pointer;
  -webkit-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

/* The actual popup */
.popup .popuptext {
  visibility: hidden;
  width: 200px;
  background-color: #090963;
  color: #fff;
  text-align: left;
  border-radius: 6px;
  padding: 8px 0;
  padding-left:20px;
  position: absolute;
  z-index: 1;
  bottom: 125%;
  left: 0%;
  margin-left: -80px;
}

/* Popup arrow */
.popup .popuptext::after {
  content: "";
  position: absolute;
  top: 100%;
  left: 70%;
  margin-left: -5px;
  border-width: 5px;
  border-style: solid;
  border-color: #555 transparent transparent transparent;
}

/* Toggle this class - hide and show the popup */
.popup .show {
  visibility: visible;
  -webkit-animation: fadeIn 1s;
  animation: fadeIn 1s;
}

/* Add animation (fade in the popup) */
@-webkit-keyframes fadeIn {
  from {opacity: 0;} 
  to {opacity: 1;}
}

@keyframes fadeIn {
  from {opacity: 0;}
  to {opacity:1 ;}
}



@media only screen and (max-width: 800px) {
th, td { font-size:11px; }
#td0 {width:90px}
#td1 {width:100px;}
#td2 {width:40px;}
#td3 {width:80px}
.popup .popuptext {width: 160px;}

tr {height:35px;} 
.btn { padding: 5px 18px; font-size:10px;}
}
</style>

<script type="text/javascript" src="SECURITY"></script>
</head>

<body onload='loadScript()'>

<div id='msect'>
<div id='menu'>
<a id="ml" href='/MENU' style='float:right; display:none'>menu</a>
</div>
</div>
<div id='msect'>
  <kop>ESP32C3 P1 METER</kop>
</div>
<div id='msect'>
    <div class='divstijl' id='maindiv'>
        <center>
        
        <h4>ELECTRICITY / GAS @ <span id="time"></span></h4>
        
        <div id='noOutput' style='display:block'>
          <h4 style='color:red'>waiting for output</h4>
        </div>
        
        <div id="4channel">
          <center><table>
          <tr style='Background-color:lightblue; font-weight:bold; text-align:center; border:4px solid black;'>
          <td id="td0">MEDIUM<td id="td1">AMOUNT<td id="td2">UNIT<td id="td3">DETAILS</tr>
          <tr><td>energy usage</td><td id="p01"></td><td>kWh</td><td><div class='popup'><button class='btn' id='btn1'>details</button><span class='popuptext' id='myPopup'></span><div></tr>
          <tr><td>energy return</td><td id="p11"></td><td>kWh</td><td><button class='btn' id='btn2'>details</button></tr>
          <tr><td>actual power</td><td id="p21"></td><td>W</td><td><button class='btn' id='btn3'>details</button></tr>
          <tr><td>gas consumed</td><td id="p31"></td><td>m3</td><td id="p30"></tr>
        </table>
        </div>  
        <p>Powered by Hansiart</p>
             
        </center>
    </div>
<script type="text/javascript" src="JAVASCRIPT"></script>
</div></body>

</html>
)=====";

const char JAVA_SCRIPT[] PROGMEM = R"=====(

var btn1 = document.getElementById('btn1');
var btn2 = document.getElementById('btn2');
var btn3 = document.getElementById('btn3');
var popup = document.getElementById("myPopup");
console.log("btn1 = " + btn1);
var ECON_HT;
var ECON_LT;
var ERET_HT;
var ERET_LT;
var PACTUAL_CON;
var PACTUAL_RET;

function loadScript() {
 init() 
 getData();
}

function init() {
  btn2.addEventListener('click', pop2);
  btn3.addEventListener('click', pop3);
  btn1.addEventListener('click', pop1);
  document.removeEventListener('click', hidePopup);
}
function change() {
  btn1.removeEventListener('click', pop1);
  btn2.removeEventListener('click', pop2);
  btn3.removeEventListener('click', pop3);
  document.addEventListener('click', hidePopup);
}
function pop1() {
  var popup = document.getElementById("myPopup");
  popup.classList.toggle("show");
  popup.innerHTML="ENERGY USAGE<br>  high tariff : " + ECON_HT + " kWh<br>  low tariff: " + ECON_LT + " kWh";
  setTimeout(change,200);
}
function pop2() {
  var popup = document.getElementById("myPopup");
  popup.classList.toggle("show");
  popup.innerHTML="ENERGY RETURN<br>  high tariff : " + ERET_HT + " kWh<br>  low tariff: " + ERET_LT + " kWh";
  setTimeout(change,200);
}
function pop3() {
  var popup = document.getElementById("myPopup");
  popup.classList.toggle("show");
  popup.innerHTML="ACTUAL POWER<br>  usage : " + PACTUAL_CON + " W<br>  return : " + PACTUAL_RET + " W";
  setTimeout(change,200);
}
function hidePopup(a) {
  var popup = document.getElementById("myPopup");
   //popup.style.visibility='hidden';
   popup.classList.toggle("show");
   init();
   //setTimeout(init,0);
   }

window.addEventListener('visibilitychange', () =>{
    if (document.visibilityState === 'visible') {
    //window.location.reload();}
    getData();
    }
})

function celbgc(cel) {
     document.getElementById(cel).style = "background-color:#c6ff1a";  
}
function celbga(cel) {
     document.getElementById(cel).style = "background-color:#f59b0a";  
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
//{"ECON_HT":0,"ECON_LT":51.775,"ERET_HT":0,"ERET_LT":24.413,"PACTUAL_CON":0,"PACTUAL_RET":0,"enR":24.413,"enC":51.775,"aPo":0,"gAs":16.713,"rm":0}
      ECON_HT = obj.ECON_HT;
      ECON_LT = obj.ECON_LT;
      ERET_HT = obj.ERET_HT;
      ERET_LT = obj.ERET_LT;
      PACTUAL_CON = obj.PACTUAL_CON; 
      PACTUAL_RET = obj.PACTUAL_RET;    
      var enC = obj.enC;
      var enR = obj.enR;
      var aPo = obj.aPo;
      //var gAa = obj.gAs;
      var rem = obj.rm;
      if(rem == 0) {document.getElementById("ml").style.display = "block";} // hide menu link         
      cel="p01";
      if(enC < 0 ) celbga(cel); else celbgc(cel);
      if(obj.enC != "n/a") {
           document.getElementById(cel).innerHTML = enC.toFixed(3);
           } else {
           document.getElementById(cel).innerHTML = "n/a";
           } 

      cel="p11";
      if(enR < 0 ) celbga(cel); else celbgc(cel);
      if(obj.enR != "n/a") {
           document.getElementById(cel).innerHTML = enR.toFixed(3);
           } else {
           document.getElementById(cel).innerHTML = "n/a";
           }      

      cel="p21";
      if(aPo < 0 ) celbga(cel); else celbgc(cel);
      if(obj.aPo != "n/a") {
           document.getElementById(cel).innerHTML = aPo;
           } else {
           document.getElementById(cel).innerHTML = "n/a";
           } 
      cel="p31";
      celbgc(cel);
      if(obj.gAs != "n/a") {
           document.getElementById(cel).innerHTML = obj.gAs.toFixed(3);
           } else {
           document.getElementById(cel).innerHTML = "n/a";
           }     

      document.getElementById("noOutput").style.display = "none";

      document.getElementById("time").innerHTML = obj.timestamp;
      }
  }
  xhttp.open("GET", "/get.Data", true);
  xhttp.send();
}


if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
  if(e.data == "getall") {
  getData();
  }

 }, false);
}

)=====";
