 #include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Hash.h>
#include <WiFiClientSecure.h>
#include <Ticker.h>
int checker1;
int k = 0;
int zz = 0;
int mode1;
int test1;
int LED1_status;
int UG_user = 90;
int UG_length = 50;
int OH_length;
int con_user;
float distance = 0;
int value;
ESP8266WiFiMulti    WiFiMulti;
ESP8266WebServer    server(80);
WebSocketsServer    webSocket = WebSocketsServer(81);

#include <ArduinoJson.h>
#define LED1 16
#define LED2 12
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;
String GAS_ID = "AKfycbx9QnO3qUJditCvlrsozgO9CzuTwCZXWj726h0INjQI--xfbQbN";
void send_sensor();
char html_template[] PROGMEM = R"=====(

<!DOCTYPE html>
 <html>
   <body>
     <center>
       <br>
       <h1 style='color:  #1c203b; font-weight: 20px;border: 3px solid #800080;background-color:#e4dfe0'> PalmShade Hospital </h1>
       <br>
       <h2 style='color:  #f5f2f2;font-size:18px; font-weight: lighter;'>Underground Motor Control</h2>
       <br>
       <h3> Motor </h3>
       <br>
       <button id="but1" onclick="button_1_on()">On</button>
       <label></label>
       <button id="but2" onclick="button_1_off()">Off</button>
       <br>
       <br>
       <label for="UG">UnderGround Tank</label>
       <br>
       <input type="number" id="UG" name="UG">
       <br>
       <br>
       <label>Threshold is set at</label>
       <label id="toDoContainer"></label>
       <br>
       <br>
       <button onclick="button3()">Set</button>
       <div style="text-align: center;">
         <br>
         <h3>Tank Meter</h3>
      
       <meter value="0" min="0" max="200" id="sensor_meter"></meter>
       <h3 id="sensor_value" style="display: inline-block;">  0 </h3>
       <br>
       <button id="MO" onclick="Mfun()">Manual</button>
       <button id="DM" onclick="myFunction()">Dark Mode</button>
       <br>
       <br>
       <button>
         <a href='https://docs.google.com/spreadsheets/d/16eYNuYUn9b_OIwkkaLYzcUyh3fn7tc5XTC3Xr6NlkQ8/edit#gid=0' target="_blank" style=" color:#7e35cc; text-decoration: none;">Analytics</a>
       </button>
       <button>
         <a href='http://192.168.29.101/' target="_blank" style=" color:#7e35cc; text-decoration: none;">OverHead Control</a>
       </button>
       <br>
       <br>
       <footer>&copy; 2022 Copyright Adithya SM</footer>
     </center>
     </div>
     <style>
       .dark-mode {
         background-color: black;
         color: white;
       }

       meter {
         width: 420px;
         height: 35px;
         padding-left: 20px;
       }

       button {
         width: 150px;
         height: 50px;
         color: #105694;
         background-color: beige;
         border-radius: 8px;
         box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
         box-shadow: 0 12px 16px 0 rgba(0, 0, 0, 0.24), 0 17px 50px 0 rgba(0, 0, 0, 0.19);
         border: 3px solid #800080;
       }

       body {
         color: #e4dfe0;
         background-color: #1c203b;
         font-family: "Sen", sans-serif;
         font-weight: 2px;
       }

       h2,
       h1,
       h3 {
         font-family: 'Karla', sans-serif;
         font-weight: 2px;
       }

       h3 {
         color: azure;
       }

       * {
         margin: 0px;
         padding: 0px;
       }

       body {
         font-family: 'Exo', sans-serif;
       }

       .dummy {
         position: fixed;
       }

       .context {
         width: 100%;
         position: absolute;
         top: 50vh;
       }
     </style>
     <script>
       var connection = new WebSocket('ws://' + location.hostname + ':81/');
       var yy = 0;
       var UG = document.getElementById('UG');
       var OH = document.getElementById('OH');
       var but3 = document.getElementById('but3')
       var MO = document.getElementById('MO')
       var todocntainer = document.getElementById('toDoContainer');
       var button_1_status = 0;
       var button_2_status = 0;
       var UG_Per = 0;
       var OH_Per = 0;
       var mode1 = 0;
       var sens_data = 0;
       var xc = 0;
       connection.onmessage = function(event) {
         var full_data = event.data;
         console.log(full_data);
         var data = JSON.parse(full_data);
         sens_data = data.sens;
         document.getElementById("sensor_meter").value = sens_data;
         document.getElementById("sensor_value").innerHTML = sens_data;
       }

       function myFunction() {
         var DM = document.getElementById('DM')
         if (DM.innerHTML == "Dark Mode") {
           DM.innerHTML = "Normal Mode";
           yy = 1;
         } else if (DM.innerHTML == "Normal Mode") {
           DM.innerHTML = "Dark Mode";
           yy = 0;
         }
         var element = document.body;
         element.classList.toggle("dark-mode");
       }

       function Mfun() {
         var elem = document.getElementById("MO");
         console.log("clicked1");
         if (xc == 0) {
           elem.innerHTML = "Manual";
           mode1 = 0;
           xc = 1;
           console.log("working");
         } else {
           elem.innerHTML = "Auto";
           mode1 = 1;
           xc = 0;
           console.log("still working");
         }
         send_data();
       }

       function button3() {
         console.log("clicked");
         todocntainer.textContent = `${UG.value}%`;
         UG_Per = UG.value;
         UG.value = "";
         console.log(UG_Per);
         send_data();
       }

       function button_1_on() {
         but1.style.background = "#C4DDFF";
         but2.style.background = "beige";
         button_1_status = 1;
         console.log("LED 1 is ON");
         send_data();
       }

       function button_1_off() {
         but1.style.background = "beige";
         but2.style.background = "#C4DDFF";
         button_1_status = 0;
         console.log("LED 1 is OFF");
         send_data();
       }

       function button_2_on() {
         button_2_status = 1;
         console.log("LED 2 is ON");
         send_data();
       }

       function button_2_off() {
         button_2_status = 0;
         console.log("LED 2 is OFF");
         send_data();
       }

       function send_data() {
         var full_data = '{"LED1":' + button_1_status + ',"LED2":' + button_2_status + ',"UG":' + UG_Per + ',"OH":' + OH_Per + ',"mode":' + mode1 + '}';
         connection.send(full_data);
         console.log(full_data);
       }
     </script>
   </body>
 </html>
)=====";
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
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
    String message = String((char * )(payload));
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

LED1_status = doc["LED1"];
con_user = doc["LED1"];
UG_user = doc["UG"];
mode1 = doc["mode"];
test1 = doc["LED1"];

  }
}
/*IPAddress local_IP(192,168,29,101);//ipconfig
IPAddress gateway(192,168,29,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);*/


IPAddress local_IP(192,168,56,100);//ipconfig
IPAddress gateway(192,168,56,183);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);
void setup() {
  client.setInsecure();
  pinMode(16, OUTPUT);
  pinMode(12, OUTPUT); //D6
  pinMode(14, INPUT); //D5
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  Serial.begin(115200);
  delay(1000);

  WiFiMulti.addAP("jio upstair", "Adithya1");
  WiFiMulti.addAP("adi", "qwerty12345");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);

  }
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("failed");
  }

  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();

}

void handleMain() {
  server.send_P(200, "text/html", html_template);
}
void handleNotFound() {
  server.send(404, "text/html", "<html><body><p>404 Error</p></body></html>");
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
  distance = (duration * .0343) / 2;
  //Serial.print("Distance: ");
  //Serial.println(distance);
  int checker1;
  if (mode1 == 0) {
    checker1 = (UG_length * (UG_user) / 100) - 5;
    if ((distance > 35)) {
      digitalWrite(16, HIGH);
      //pin for trigger
    } else if ((distance < 35)) {
      digitalWrite(16, LOW);
    }

  } else if (mode1 == 1) {
    digitalWrite(16, test1);
    Serial.println(test1);
  }
  String JSON_Data = "{\"sens\":";
  JSON_Data += int(distance);
  JSON_Data += "}";

  webSocket.broadcastTXT(JSON_Data);

  
  int h = 1;

  float t = distance;

  String Temp = "x : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";

  sendData(t, h);

}

void sendData(float tem, int hum) {

  client.connect("script.google.com", 443);
  // if (!client.connect("script.google.com",443)) {
  //Serial.println("connection failed");

  // }

  String string_temperature = String(tem);

  if (zz == 60) {

    String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + "";
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "User-Agent: BuildFailureDetectorESP8266\r\n" +
      "Connection: close\r\n\r\n");

    Serial.println("request sent");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");

      }
    }
    zz = 0;
  } else if (zz < 60) {
    zz = zz + 1;
  }

  /*String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  k=0;*/
}
