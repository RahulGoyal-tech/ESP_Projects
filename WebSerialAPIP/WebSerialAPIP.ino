/*
  This sketch is based on the WebSerial library example: ESP8266_Demo
  https://github.com/ayushsharma82/WebSerial
*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define LED 2

AsyncWebServer server(80);

String ssid = "Node";          // Your WiFi SSID

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "ON"){
    digitalWrite(LED, LOW);
  }
  if (d=="OFF"){
    digitalWrite(LED, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid);
  Serial.println("IP Address: ");
  Serial.println(WiFi.softAPIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}

void loop() {
  WebSerial.println("Hello!");
  delay(2000);
}
