//<script type="text/javascript" src="SECURITY"></script>
const char REPORTPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP-P1 METER</title>
<meta http-equiv="refresh" content="60">
<meta name="viewport" content="width=device-width, initial-scale=1">

<link rel="stylesheet" type="text/css" href="/STYLESHEET_HOME">
<style>

#tb {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 480px;
}

#tb th {
  border: 1px solid blue;
  text-align: center;
  padding: 10px;
  background-color: #4CAF50;
  color: white;
}

#tb td  {
padding-left: 10px;
border: 1px solid;
}
tr:nth-child(even) {
  background-color: #dddddd;
}

.th1 { width:12%%%; }
.th2 { width:17%; }

tr {height:26px;}

@media only screen and (max-width: 600px) {
#tb{ font-size:12px; width: 320px;}
tr {width:94vw;}
.th1 { width:10%%; }
.th2 { width:15%%; }
}
</style>
<script>
function cl() {
window.location.href='/MENU';
}

function loadScript() {
const d = new Date();
let row = "row" + (d.getMonth()+1);
document.getElementById(row).style.backgroundColor='#ff66ff';
}
</script>
</head><body onload="loadScript()"><center>
<ul>
<li id='fright'><span class='close' onclick='cl();'>&times;</span>
<li><a href='#' onClick='window.location.reload()'>refresh</a></li>
</ul>
<h2>ESP-P1 METER REPORT</h2>
<div class='divstijl'>
<table id='tb'><tr><tr>
<th class='th1'>mth
<th class='th2' colspan=2> kWh USAGE</th>
<th class='th2' colspan=2> kWh RETURN</th>
<th>GAS</th>
<tr><th></th>
<th class='th2'>lt</th>
<th class='th2'> ht</th>
<th class='th2'>lt</th>
<th class='th2'>ht</th>
<th class='th2'>m3</th></tr>
%rows%
</table><p> the value in the current month is until now.</p></center></div></div>
</body></html>
)=====";
