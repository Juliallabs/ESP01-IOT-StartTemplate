#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
//#include <ESPAsyncWebServer.h>
#include "ota.h"



#ifndef STASSID
#define STASSID "login"
#define STAPSK  "senha"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
IPAddress ip(xxx,xxx,x,xxx);
IPAddress gateway(xxx,xxx,x,xxx);
IPAddress subnet(xxx,xxx,x,xxx);

ESP8266WebServer server(80);

const int led = 0;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!\r\n");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  setupOTA();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/online", []() {
    server.send(200, "text/plain", "this works as well");
  });

    server.on("/LIGAR", []() {
    server.send(200, "text/plain", "ALOOOO GALERA TO VIVO");
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
  });

  server.on("/gif", []() {
    
  digitalWrite(led, 1);
  server.send(200, "text/plain", "LIGOU!\r\n");
  delay(500);
  digitalWrite(led, 0);
  
 });

  server.onNotFound(handleNotFound);

 
  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
   ArduinoOTA.handle();
  server.handleClient();
  MDNS.update();

  // put your main code here, to run repeatedly:
}