/* http://domoticx.com/arduino-p1-poort-telegrammen-uitlezen/
 * http://domoticx.com/p1-poort-slimme-meter-hardware/
 * https://github.com/romix123/P1-wifi-gateway/blob/main/src/P1WG2022current.ino
 * http://www.gejanssen.com/howto/Slimme-meter-uitlezen/#mozTocId935754
 * 
 * 
 in this version we first read the whole telegram in an array and try to verify the crc
 platform: ESP32 c3 dev
 partition scheme: minimal spiffs with ota
 board version 3.0.7 ?? other versions won't compile


*/

#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>

//#include <ESPAsyncWebServer.h>
//#include "BluetoothSerial.h"
//#include <SoftwareSerial.h>
#include "driver/uart.h"


#include <esp_wifi.h>   
#include <WiFi.h>
#include <DNSServer.h>
#include "OTA.H"
#include <Update.h>
#include <Hash.h>

#define VERSION  "ESP32C3-P1METER-v0_0"

#include <TimeLib.h>
#include <time.h>
#include <sunMoon.h>

#include "soc/soc.h" // ha for brownout
#include "soc/rtc_cntl_reg.h" // ha for brownout
#include <esp_task_wdt.h> // ha

#include <SPIFFS.h>
#include "FS.h"

#include <EEPROM.h>

#include <ArduinoJson.h>
#include <Arduino.h>
#include <AsyncTCP.h>

AsyncWebServer server(80); //
AsyncEventSource events("/events"); 
AsyncWebSocket ws("/ws");

#include <PubSubClient.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;

#include "CRC16.h"

#include "HTML.H"
#include "AAA_MENUPAGE.H"
#include "AAA_HOMEPAGE.H"
#include "PORTAL_HTML.H"
#include "REPORT.H"

  char ssid[33] = ""; // was 33 
  char pass[64] = ""; // was 40
  bool tryConnectFlag = false;

 // variables wificonfig
  char pswd[11] = "0000";
  char userPwd[11] = "1111";  
  float longi = 5.123;
  float lati = 51.123;
  char gmtOffset[5] = "+120";  //+5.30 GMT
  bool zomerTijd = true;


  char requestUrl[12] = {""}; // to remember from which webpage we came  

// variables mqtt ********************************************
  char  Mqtt_Broker[30]=    {"192.168.0.100"};
  char  Mqtt_outTopic[26] = {"domoticz/in"}; // was 26
  char  Mqtt_Username[26] = {""};
  char  Mqtt_Password[26] = {""};
  char  Mqtt_Clientid[26] = {""};
  int   Mqtt_Port =  1883;
  int   Mqtt_Format = 0;

// variables domoticz ********************************************
  char dom_Address[30]={"192.168.0.100"};
  int  dom_Port =  8080;
  int  gas_Idx = 123;
  int  el_Idx = 456;

  int event = 0;
  
  int dst;
  bool timeRetrieved = false;
  int networksFound = 0; // used in the portal
  int datum = 0; //

  unsigned long previousMillis = 0;        // will store last temp was read
  static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  static unsigned long lastCheck = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  
#define LED_AAN    LOW   //sinc
#define LED_UIT    HIGH
#define knop              0  //
#define led_onb           8  // onboard led was 2
#define P1_ENABLE         5 // gpio5 if high the RX of the meter is pulled high

String toSend = "";
 
int value = 0; 
int aantal = 0;



WiFiClient espClient ;
PubSubClient MQTT_Client(espClient) ;

int Mqtt_stateIDX = 0;

// Vars to store meter readings
float ECON_LT = 0; //Meter reading Electrics - consumption low tariff
float ECON_HT = 0; //Meter reading Electrics - consumption high tariff
float ERET_LT = 0; //Meter reading Electrics - return low tariff
float ERET_HT = 0; //Meter reading Electrics - return high tariff
float PACTUAL_CON = 0;  //Meter reading Electrics - Actual consumption
float PACTUAL_RET = 0;  //Meter reading Electrics - Actual return
float mGAS = 0;  //Meter reading Gas
long prevGAS = 0;

// setup a struct for the values of each month
typedef struct{
  float EC_LT = 0;
  float EC_HT = 0;
  float ER_LT = 0;
  float ER_HT = 0;
  float mGAS  = 0;
} m_values; 
m_values MVALS[13]; 

//unsigned int currentCRC = 0; //needs to be global?

bool diagNose = true;
int actionFlag = 0;
char txBuffer[50];
bool USB_serial = true;
int meterType = 0;
bool polled = false;
bool Polling = false;
int pollFreq = 300;
// voor testing
int www = 0;

char teleGram[1060]={"not polled"};
char readCRC[5];
int testLength;
int errorCode = 0;
int securityLevel = 0;
//bool tested = false;
//char logChar[150] = {"log: "};
#define P1_MAXLINELENGTH 1050
//t currentCRC=0;
//t tests = 0; // to prevent for an endless loop
//ol endsign = false;
bool testTelegram = false;
char timeStamp[12]={"not polled"};
// bluetooth settings and vars
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//bool blueTooth = false;
//BluetoothSerial SerialBT;
//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
//const char *pin = "9999"; // Change this to more secure PIN.


// *****************************************************************************
// *                              SETUP
// *****************************************************************************
//#include <SoftwareSerial.h>
// EspSoftwareSerial::UART p1Serial;
 // Define the RX and TX pins for Serial 2
//HardwareSerial p1Serial(1); // doesn't work
//#define RXP1 9
//#define TXP1 10
#define RXP1 20
#define TXP1 21
//#define GPS_BAUD 115200

void setup() {

  Serial1.setRxBufferSize(1024);
  Serial1.begin(115200, SERIAL_8N1, 3, 2);
  uart_set_line_inverse(UART_NUM_1, UART_SIGNAL_RXD_INV);
  //USC0(UART1) = USC0(UART1) | BIT(UCRXI); // swap the rx to inverted
  delay(100);
  Serial.begin(115200);
  Serial.println("The device started");
  Serial.println("Serial1 started at 115200 baud rate");
  
  pinMode(knop, INPUT_PULLUP); // the button gpio4 D2 
  pinMode(led_onb, OUTPUT); // onboard led gpio0 D3

  pinMode(P1_ENABLE, OUTPUT);// with this pin gpio14 (D5)  
  digitalWrite(P1_ENABLE, LOW);
  ledblink(1, 800);
  
  attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);

  SPIFFS_read();
 
// takes care for the return to the last webpage after reboot
  read_eeprom(); // put the value of requestUrl back
  if (requestUrl[0] != '/' ) strcpy(requestUrl, "/");  // vangnet
  Serial.println("\n\nrequestUrl = " + String(requestUrl));
 ledblink(3,30); 
 
  // poll the meter at once so that we have a debugfile when there is no wifi
//  if( !SPIFFS.exists("/logChar.txt") && !SPIFFS.exists("/testFile.txt") ) {
//      Serial.println("performing a test meterpoll");
//      meterPoll(); // poll the meter and when we don't have testfiles, write them
//   }

  start_wifi(); // start wifi and server

  getTijd(); // retrieve time from the timeserver

// reed monthly values files
  for (int x=1; x < 13; x++) 
  {
  String bestand = "/mvalues_" + String(x) + ".str";  
       readStruct(bestand, x);
//      if(!readStruct(bestand, x)) {
//      Serial.println("populate struct with null");  
//      MVALS[x].EC_LT = 000000.00;
//      MVALS[x].EC_HT = 000000.00;
//      MVALS[x].ER_LT = 000000.00;
//      MVALS[x].ER_HT = 000000.00;
//      MVALS[x].mGAS =  00000.00;       
//  }
}

//  // ****************** mqtt init *********************
       MQTT_Client.setKeepAlive(150);
       MQTT_Client.setBufferSize(512);
       MQTT_Client.setServer(Mqtt_Broker, Mqtt_Port );
       MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;

  if ( Mqtt_Format != 0 ) 
  {
       Serial.println(F("setup: mqtt configure"));
       mqttConnect(); // mqtt connect
  } 
  else 
  {
#ifdef LOG
   Update_Log(3, "not enabled"); 
#endif 
  }
  initWebSocket();

  Serial.println(F("booted up"));
  Serial.println(WiFi.localIP());

  delay(1000);
  ledblink(3,500);

  eventSend(0);
  if ( !timeRetrieved )   getTijd();

//  meterPoll(); //we do the first poll
} // end setup()

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop() {


//if(moetZenden) send_testGram();
//              polling every 60 seconds 
// ******************************************************************

  unsigned long nu = millis();  // the time the program is running
//   if (nu - laatsteMeting >= 1000UL * 300 
   if (nu - laatsteMeting >= 1000UL * pollFreq) 
   {
     if(diagNose) {
     String toLog = String(pollFreq) + " seconds passed";
      ws.textAll( toLog );
     }
       laatsteMeting += 1000UL * pollFreq ; // 
       // the p1 meter only transmits when the rx line = high
       digitalWrite(P1_ENABLE, HIGH);
       if(Polling) meterPoll(); 
       digitalWrite(P1_ENABLE, LOW);
 }
//// ******************************************************************
////     healthcheck every 10 minutes but 2 minutes later than poll 
//// ******************************************************************
////   nu = millis() + 1000UL * 120 ; // 120 sec = 12 minutes later
//   if (nu - lastCheck >= 1000UL * 600) // was 600
//   {
//         //ws.textAll("600 secs passed" + String(millis()) );
//         lastCheck += 1000UL * 600;
//         healthCheck(); // checks zb radio, mqtt and time, when false only message if error
//   }

  // when there is a new date the time is retrieved
  // if retrieve fails, day will not be datum, so we keep trying
  if (day() != datum) // if date overflew 
  { 
       if(day() < datum) {
        // its the 1st of the new month 
        // the struct goes from 0 - 11
        writeMonth(month()); // this writes at the start of the new month
        // to prevent repetition we opdate datum
        datum = day();
       }
     
       getTijd(); // retrieve time and recalculate the switch times
  }
  
  // we do this before sending polling info and healthcheck
  // this way it can't block the loop
  if(Mqtt_Format != 0 ) MQTT_Client.loop(); //looks for incoming messages
 
  //*********************************************************************

  
  test_actionFlag();
  
//  if( Serial.available() ) {
//    empty_serial(); // clear unexpected incoming data
//   }

   ws.cleanupClients();
   yield(); // to avoid wdt resets

//  SERIAL: *************** check if there is data on serial **********************
  if(Serial.available()) {
       handle_Serial();
   }

}
//****************  End Loop   *****************************




// ****************************************************************
//                  eeprom handlers
//*****************************************************************
void write_eeprom() {
    EEPROM.begin(24);
  //struct data
  struct { 
    char str[16] = "";
    //int mont;
  } data;

 strcpy( data.str, requestUrl ); 
    //mont = currentMonth
    EEPROM.put(0, data);
    EEPROM.commit();
}
void read_eeprom() {
    EEPROM.begin(24);

  struct { 
    char str[16] = "";
    //int mont;
  } data;

  EEPROM.get(0, data);
  //Serial.println("read value from  is " + String(data.str));
  strcpy(requestUrl, data.str);
  //currentMonth = data.mont;
  EEPROM.commit();
}

// all actions called by the webinterface should run outside the async webserver environment
// otherwise crashes will occure.
    void test_actionFlag() {
  // ******************  reset the nework data and reboot in AP *************************
    if (actionFlag == 11 || value == 11) { // 
     //DebugPrintln("erasing the wifi credentials, value = " + String(value) + "  actionFlag = " + String(actionFlag));
     delay(1000); //reserve time to release the button
     flush_wifi();
     WiFi.disconnect(true);
     //Serial.println(F("wifi disconnected"));
     ESP.restart();
  }  

    if (actionFlag == 10) { // reboot
     delay(2000); // give the server the time to send the confirm
     //DebugPrintln("rebooting");
     write_eeprom();
     ESP.restart();
  }
    // ********* reconnect mosquitto 
    if (actionFlag == 24) {
        actionFlag = 0; //reset the actionflag
        MQTT_Client.disconnect() ;
       //reset the mqttserver
        MQTT_Client.setServer ( Mqtt_Broker, Mqtt_Port );
        MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;
        if (Mqtt_Format != 0) mqttConnect(); // reconnect mqtt after change of settings
    }    

    if (actionFlag == 23) {
      actionFlag = 0; //reset the actionflag
      empty_serial();
      unsigned long nu = millis();
      consoleLog("nu before = " + String(nu) );
      waitSerialAvailable(5); // recalculate time after change of settings
      nu = millis();
      consoleLog("nu after = " + String(nu) );
      if(Serial.available()) consoleLog("there is data available");
    }    
    
    if (actionFlag == 25) {
      actionFlag = 0; //reset the actionflag
      getTijd(); // recalculate time after change of settings
    }

    if (actionFlag == 26) { //polling
      actionFlag = 0; //reset the actionflag
      meterPoll(); // read and decode the telegram
      sendMqtt(false);
      sendMqtt(true); //gas
    }    
    if (actionFlag == 126) { //polling
      actionFlag = 0; //reset the actionflag
      consoleOut("running read_test");
      read_test(); // read 100 chars in teleGram
    }

    // decode the testfile
    if (actionFlag == 28) { //
      actionFlag = 0; //reset the actionflag
      if(!testTelegram) {
        consoleLog("telegram is modified, reboot if you want to test");
        return;
      }
      // if we have te telegram read from spiffs we can decode it
      polled=true;
      //we need the readCRC so we extract it from the file
         int len = strlen(teleGram);
         strncpy(readCRC, teleGram + len-4, 4); 
         consoleLog("readCRC = " + String(readCRC) );
         decodeTelegram();
         sendMqtt(false);
         sendMqtt(true);
    }

// test the reception of a telegram at boot
// the teleGram is saved in SPIFFS

    if (actionFlag == 30) { 
      actionFlag = 0; //reset the actionflag
      if (SPIFFS.exists("/testFile.txt")) SPIFFS.remove("/testFile.txt");      
      read_into_array(); // 
      // save the outcome to the files
      testFilesave(); // an existing file is not overwritten

    }    

    if (actionFlag == 46) { //triggered by the console printout-spiffs
        consoleOut("print the files");
        actionFlag = 0; //reset the actionflag
        showDir();
        printFiles(); 
    }

     if (actionFlag == 48) { //triggered by console test-Serial1   
     actionFlag = 0;
     testMessage();
     }
    // ** test mosquitto *********************
    if (actionFlag == 49) { //triggered by console testmqtt
        actionFlag = 0; //reset the actionflag
        ledblink(1,100);
        // always first drop the existing connection
        MQTT_Client.disconnect();
        ws.textAll("dropped connection");
        delay(100);
        char Mqtt_send[26] = {0};
       
        if(mqttConnect() ) {
        String toMQTT=""; // if we are connected we do this
        
        strcpy( Mqtt_send , Mqtt_outTopic);
        
        //if(Mqtt_send[strlen(Mqtt_send -1)] == '/') strcat(Mqtt_send, String(Inv_Prop[0].invIdx).c_str());
        toMQTT = "{\"test\":\"" + String(Mqtt_send) + "\"}";
        
        if(Mqtt_Format == 5) toMQTT = "field1=12.3&field4=44.4&status=MQTTPUBLISH";
        
        if( MQTT_Client.publish (Mqtt_outTopic, toMQTT.c_str() ) ) {
            ws.textAll("sent mqtt message : " + toMQTT);
        } else {
            ws.textAll("sending mqtt message failed : " + toMQTT);    
        }
      } 
     // the not connected message is displayed by mqttConnect()
    }


} // end test actionflag


void flush_wifi() {
     consoleOut(F("erasing the wifi credentials"));
     WiFi.disconnect();
     consoleOut("wifi disconnected");
     //now we try to overwrite the wifi credentials     
     const char* wfn = "dummy";
     const char* pw = "dummy";
     WiFi.begin(wfn, pw);
     consoleOut(F("\nConnecting to dummy network"));
     int teller = 0;
     while(WiFi.status() != WL_CONNECTED){
        Serial.print(F("wipe wifi credentials\n"));
        delay(100);         
        teller ++;
        if (teller > 2) break;
    }
    // if we are here, the wifi should be wiped 
}


void eventSend(byte what) {
  if (what == 1) {
      events.send( "general", "message"); //getGeneral triggered            
  } else 
     if (what == 2) {
     events.send( "getall", "message"); //getAll triggered
  } else {  
     events.send( "reload", "message"); // both triggered
  }
}
void writeMonth(int maand) {
  //so if month overflew, the value of end 7 is in 8
//month goes from 1 - 12 buth the struct from 0 - 12
// we write all values in the struct with the number of current month -1
   MVALS[maand].EC_LT = ECON_LT ;
   MVALS[maand].EC_HT = ECON_HT ;
   MVALS[maand].ER_LT = ERET_LT ;
   MVALS[maand].ER_HT = ERET_HT ;
   MVALS[maand].mGAS    = mGAS;
// write this in SPIFFS
   String bestand = "//mvalues_" + String(maand) + ".str"; // month5.str
   writeStruct(bestand, maand);
} 

 
