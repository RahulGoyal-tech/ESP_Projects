#include <ESP8266WiFi.h>
#define led D5

WiFiClient client;
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.softAP("NodeHere");
  Serial.println();
  Serial.print("Node Mcu is started with IP: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  client = server.available();
  if(client == 1)
  {
    String request = client.readStringUntil('\n');
    Serial.println(request);
    request.trim();
    if(request == "GET /ledon HTTP/1.1")
    {
      digitalWrite(led,HIGH);
    }
    else if(request == "GET /ledoff HTTP/1.1")
    {
      digitalWrite(led,LOW);
    }
  }
}
