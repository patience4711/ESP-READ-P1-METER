
//{ check
const char MENUPAGE[] PROGMEM = R"=====(
<body>
<style>
a:link, a:visited { width: 98px;}
</style>
<script type="text/javascript" src="SECURITY"></script>
<script> function close() {window.location.href='/';} </script>
<div id='msect'>
<div id='menu'>
<a href="/" class='close' onclick='close();'>&times;</a>  
</div>  
</div>
<br><br>
<div id='msect'><kop>ESP32C3 P1 METER MENU</kop></div><br><br><br>

<div id='msect'>
<table><tr>
  <td style="width:100px"><a href='/ABOUT'>system info</a></td><td style="width:60px"></td><td style="width:100px">
  <a href='/MQTT'>mosquitto</a></<td></tr>
  
  <td style="width:100px"><a href='/API/TELEGRAM' target='new'>telegram</a></td><td style="width:60px"></td><td style="width:100px">
  <a href='/GEOCONFIG'>time config</a></<td></tr>
    
  <td style="width:100px"><a href='/REPORT'>report</a></td><td style="width:60px"></td><td style="width:100px">
  <a href='/FWUPDATE'>fw update</a></<td></tr>  
  
  <td style="width:100px"><a href='/CONSOLE'>console</a></td><td style="width:60px"></td><td style="width:100px">
  <a onclick="return confirm('are you sure?')" href='/REBOOT'>reboot</a></td></tr>

  <td style="width:100px"><a href='/BASISCONFIG'>settings</a></li></td><td style="width:60px"></td><td style="width:100px">
  <a onclick="return confirm('are you sure?')" href='/STARTAP'>start AP</a></td></tr>
  
  </table>  

  </div>
  
</body></html>
  )=====";

//} 
