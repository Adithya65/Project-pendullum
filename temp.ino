 #include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>




#define ON_Board_LED 2 
float duration, distance;
int Delay=0; //delay count (20==30sec)
int k=0;
int Total_distance=200;//length of the tank
const char* ssid = "applepie"; 
const char* password = "12345678"; 
int x=0;
const char* host = "script.google.com";
const int httpsPort = 443;


WiFiClientSecure client; 

String GAS_ID = "AKfycbx9QnO3qUJditCvlrsozgO9CzuTwCZXWj726h0INjQI--xfbQbN"; 


void setup() {
  pinMode(12,OUTPUT);//D6
pinMode(14,INPUT);//D5
pinMode(16,OUTPUT);
  Serial.begin(115200);
  delay(500);

 
  delay(500);
  
  WiFi.begin(ssid, password); 
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); 
  digitalWrite(ON_Board_LED, HIGH); 
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
 
  }
  digitalWrite(ON_Board_LED, HIGH); 
  
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  client.setInsecure();
}

void loop() {
 digitalWrite(12, LOW);
  delayMicroseconds(2);
  digitalWrite(12, HIGH);
  delayMicroseconds(10);
  digitalWrite(12, LOW);
 x=x+10;
  float duration = pulseIn(14, HIGH);
  float distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
  if (distance>30)
  {digitalWrite(16,LOW);//D0
  }
  else 
  digitalWrite(16,HIGH);
  
  int h = 1;
  
  float t =Total_distance-distance;
  
  String Temp = "x : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  Serial.println(Temp);
  Serial.println(Humi);
  
  sendData(t, h); 
}

void sendData(float tem, int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_voltage =  String(220);
  String current=String(x);
  
 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + current + "&humidity=" + "220";
  Serial.print("requesting URL: ");
  Serial.println(url);
  if (k==Delay){
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
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
  k=0;
} 
else if(k<Delay) {
k=k+1;}}
