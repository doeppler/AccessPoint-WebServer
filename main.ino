// ESP8266 - WEB SERVER (ACCESS POINT)
// 
// Author: Emirhan Gocturk
// Date: 25 April 2019
// Time: 00:37
// Description: Very basic ESP8266
//    web server and an access point.
//  Creates a WiFi network so that other
//    ESP8266s can connect.

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Network Information
const char *ssid = "Network";
const char *password = "12345678";

// IP Information
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Web Server object
ESP8266WebServer server(80);

// Pins
uint8_t led1Pin = D7;
bool led1Status = LOW; //Initial status

uint8_t buzzerPin = D6;
bool buzzerStatus = LOW; //Initial status

void setup() {
  // Setting pins
  Serial.begin(115200);
  pinMode(led1Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Establish WiFi
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // Server directories
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_Led1On);
  server.on("/led1off", handle_Led1Off);
  server.on("/buzzeron", handle_BuzzerOn);
  server.on("/buzzeroff", handle_BuzzerOff);
  server.onNotFound(handle_NotFound);

  // Start server
  server.begin();
  Serial.println("Server status: ONLINE");
}

void loop() {
  server.handleClient();
  if(led1Status)
  {digitalWrite(led1Pin, HIGH);}
  else
  {digitalWrite(led1Pin, LOW);}
  
  if(buzzerStatus)
  {digitalWrite(buzzerPin, HIGH);}
  else
  {digitalWrite(buzzerPin, LOW);}
}


// What buttons do:
void handle_OnConnect() {
  led1Status = LOW;
  buzzerStatus = LOW;
  Serial.println("ALL PINS ARE DEACTIVATED");
  server.send(200, "text/html", SendHTML(led1Status,buzzerStatus)); 
}

void handle_Led1On() {
  led1Status = HIGH;
  Serial.println("D7 Status: ON");
  server.send(200, "text/html", SendHTML(true,buzzerStatus)); 
}

void handle_Led1Off() {
  led1Status = LOW;
  Serial.println("D7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,buzzerStatus)); 
}

void handle_BuzzerOn() {
  buzzerStatus = HIGH;
  Serial.println("D6 Status: ON");
  server.send(200, "text/html", SendHTML(led1Status,true)); 
}

void handle_BuzzerOff() {
  buzzerStatus = LOW;
  Serial.println("D6 Status: OFF");
  server.send(200, "text/html", SendHTML(led1Status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

// Web Page (Control Panel)
String SendHTML(uint8_t led1Stat,uint8_t buzzerStat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Control Center</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #000000;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #FF4F00;}\n";
  ptr +=".button-on:active {background-color: #FF4F00;}\n";
  ptr +=".button-off {background-color: #c53e02;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Control Center</h1>\n";
  ptr +="<h3>Access Point Mode</h3>\n";
  
   if(led1Stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(buzzerStat)
  {ptr +="<p>Buzzer Status: ON</p><a class=\"button button-off\" href=\"/buzzeroff\">OFF</a>\n";}
  else
  {ptr +="<p>Buzzer Status: OFF</p><a class=\"button button-on\" href=\"/buzzeron\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
