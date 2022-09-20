#include <ESP8266WiFi.h>

String host = "google.com";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Connecting To WiFi");
  WiFi.begin("WIFINAME","WIFIPASS");
  while(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("WiFi Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client;
  int httpPort = 80;
  if(!client.connect(host,httpPort)){
    Serial.println("Connection With Google Failed");
    return;
  }
  String url = "/search?q=Arduino";
  Serial.println("Sending Request To Google");

  client.print(String("GET ") + url + "HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  long time = millis();
  while(client.available() == 0)
  {
    if((millis()-time)>5000)
    {
      Serial.println("Timed Out");
      client.stop();
      return;
    }
  }
  while(client.available())
  {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}
