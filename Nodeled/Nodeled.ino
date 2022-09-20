#include<ESP8266WiFi.h>

#define led D5

WiFiClient client;
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin("WIFINAME", "WIFIPASS");
  while(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print("##");
  }
  Serial.println();
  Serial.println("Node MCU Connected");
  Serial.println(WiFi.localIP());
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
      Serial.print("ON");
      digitalWrite(led, HIGH);
    }
    else if(request == "GET /ledoff HTTP/1.1")
    {
      Serial.print("OFF");
      digitalWrite(led, LOW);
    }
  }
}
