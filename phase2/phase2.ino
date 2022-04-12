  float distance =0; 
 int UG_user;
 int UG_length;
 int OH_length;
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Hash.h>
int checker1;
#include <Ticker.h>
int i=0;
String zx;
String old_value, value;

ESP8266WiFiMulti    WiFiMulti;
ESP8266WebServer    server(80);
WebSocketsServer    webSocket = WebSocketsServer(81);

#include <ArduinoJson.h>
 //https://github.com/sstaub/Ticker


#define LED1 LED_BUILTIN
#define LED2 12


void send_sensor();
 
char html_template[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<script>

var connection = new WebSocket('ws://'+location.hostname+':81/');

let but3=document.getElementById('but3')

var button_1_status = 0;
var button_2_status = 0;
var UG_Per=0;
var OH_Per=0;
var sens_data = 0;
connection.onmessage = function(event){

  var full_data = event.data;
  console.log(full_data);
  var data = JSON.parse(full_data);
  sens_data = data.sens;
  document.getElementById("sensor_meter").value = sens_data;
  document.getElementById("sensor_value").innerHTML = sens_data;
  var UG=document.getElementById('UG');
var OH=document.getElementById('OH');
  
}
function button3()
{
 UG_Per=UG.value;
OH_Per=OH.value;
OH.value="";
UG.value="";


 


send_data();

}

 
function button_1_on()
{
   button_1_status = 1; 
  console.log("LED 1 is ON");
  send_data();
}

function button_1_off()
{
  button_1_status = 0;
console.log("LED 1 is OFF");
send_data();
}

function button_2_on()
{
   button_2_status = 1; 
  console.log("LED 2 is ON");
  send_data();
}

function button_2_off()
{
  button_2_status = 0;
console.log("LED 2 is OFF");
send_data();
}


function send_data()
{
  var full_data = '{"LED1" :'+button_1_status+',"LED2":'+button_2_status+',"UG":'+UG_Per+',"OH":'+OH_Per+'}';
  connection.send(full_data);
  console.log(full_data);
}


</script>
<body>
     <link rel="stylesheet" href="style.css">
    <script src="ee1.js" defer></script>
    <center>
    <h1><b>PalmShade Hospital</b></h1>
    <br>
    
    <h3> Motor 1 </h3>
    <button onclick= "button_1_on()" >On</button><label>       </label><button onclick="button_1_off()" >Off</button>
    <h3> Motor 2 </h3>
    <button onclick="button_2_on()">On</button><label>       </label><button onclick="button_2_off()">Off</button>
    
    <label>     </label>
    <br>
    
    <label for="UG">UnderGround Tank</label><br>
  <input type="number" id="UG" name="UG"><br><br>
  <label style="padding-left:0px" for="OH">OverHead Tank     </label><br>
  <input type="number" id="OH" name="OH"><br><br>
    <button onclick="button3()"   >Set</button>
     
    </center>
    
    <div style="text-align: center;">
      <br>

    <h3>Tank Meter</h3>
    <meter value="0" min="0" max="200" id="sensor_meter"> </meter><h3 id="sensor_value" style="display: inline-block;"> 2000 </h3>
    <style>
    meter{  
      width: 420px;  
      height: 35px;  
      padding-left: 20px;
      
    }  
    button{
      width: 150px;  
      height: 50px; 
      color:#105694;
      background-color:beige;
      border-radius: 8px;
      box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);
      box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24), 0 17px 50px 0 rgba(0,0,0,0.19);
      border: 2px solid #105694;
    }
    body {
      padding-top: 10px;
      background-image: linear-gradient(to right, rgba(255, 238, 0, 0), rgb(124, 188, 228));
      color:indigo;

    }
     
  </style>
    </body>
    </html>
)=====";



void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);

      
     DynamicJsonDocument doc(200);
    // deserialize the data
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
      // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  int LED1_status = doc["LED1"];
  int LED2_status = doc["LED2"];
   UG_user=doc["UG"];
  digitalWrite(LED1,1-LED1_status);
  digitalWrite(LED2,LED2_status);
  




  }
}
void setup() {
pinMode(12,OUTPUT);//D6
pinMode(14,INPUT);//D5
pinMode(LED1,OUTPUT);
pinMode(LED2,OUTPUT);
  Serial.begin(115200);
  delay(1000);

  WiFiMulti.addAP("jio upstair", "Adithya1");
  WiFiMulti.addAP("adi", "qwerty12345");
  

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
      
 
  }
   

  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
   
  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();
    

}

void handleMain() {
  server.send_P(200, "text/html", html_template ); 
}
void handleNotFound() {
  server.send(404,   "text/html", "<html><body><p>404 Error</p></body></html>" );
}

void loop() {
  
  webSocket.loop();
  server.handleClient();
  digitalWrite(12, LOW);
  delayMicroseconds(2);
  digitalWrite(12, HIGH);
  delayMicroseconds(10);
  digitalWrite(12, LOW);

  float duration = pulseIn(14, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  int checher1;
  checker1=UG_length*(UG_user)/100;
  if (distance<checker1)
  {

    //pin for trigger
  }
  

     
  String JSON_Data = "{\"sens\":";
         JSON_Data += int(distance);
         JSON_Data += "}";
    
  webSocket.broadcastTXT(JSON_Data);
   
   
  MDNS.update();
  
  delay(50);
  i=i+1;
}
void send_sensor()
{
   int sens = digitalRead(D5);
  // JSON_Data = {"temp":(String)distance,"hum":zx}"
  
}
