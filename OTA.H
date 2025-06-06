const char otaIndex[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<link rel="stylesheet" type="text/css" href="/STYLESHEET_HOME">
<style> 
input {
  font-size:3vw;
}  
</style>
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
<script>
function showReboot() {

document.getElementById('reboot').style.display="block";
}
</script>

</head><body>
<center><h2>ESP32 C3 FIRMWARE UPDATE</h2><br><br>
if the upload was successful the reboot button will appear.<br><br><br>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
<input type='file' name='update'>
<input type='submit' value='Update'>
</form><br><br><br>
 <div id='prg'>progress: 0%</div><br><br><br>
 <br><br><a id="reboot" style="display:none; font-size:30px;" onclick="return confirm('reboot?')" href='/REBOOT'>REBOOT</a>
<br><br><br>
<a id="goback" style="font-size:30px;" href='/MENU'>CANCEL</a><br><br>

 <script>
  $('form').submit(function(e){
  e.preventDefault();
  var form = $('#upload_form')[0];
  var data = new FormData(form);
   $.ajax({
  url: '/handleFwupdate',
  type: 'POST',
  data: data,
  contentType: false,
  processData:false,
  xhr: function() {
  var xhr = new window.XMLHttpRequest();
  xhr.upload.addEventListener('progress', function(evt) {
  if (evt.lengthComputable) {
  var per = evt.loaded / evt.total;
  $('#prg').html('progress: ' + Math.round(per*100) + '%');
  }
  }, false);
  return xhr;
  },
  success:function(d, s) {
  console.log('success!');
  showReboot();
 },
 error: function (a, b, c) {
 }
 });
 });
 </script>
 </body></html>
)=====";
