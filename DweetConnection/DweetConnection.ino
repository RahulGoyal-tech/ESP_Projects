//Library
#include <ESP8266WiFi.h>

String host = "dweet.io";

void setup() {
  //Begin Serial Monitor
  Serial.begin(115200);

  //WiFi Connection
  Serial.println("");
  Serial.println("Connecting To WiFi");
  WiFi.begin("WIFINAME","WIFIPASS");
  while(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print("#");
    delay(200);
  }
  Serial.println("");
  Serial.println("Connection Successful");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Connecting To DWEET");
  WiFiClient cli;
  int httpPort = 80;
  if(!cli.connect(host,httpPort)){
    Serial.println("Connection with cloud failed");
    return;
  }
  String Url = "/dweet/for/my-thing-name?hello=world";
  Serial.print("Sending Request to Cloud");

  cli.print(String("GET ") + Url + "HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  long inittime = millis();
  while(cli.available() == 0)
  {
    if((millis() - inittime) > 5000)
    {
      Serial.println("Client Timed Out");
      cli.stop();
      return;
    }
  }
  while(cli.available())
  {
    String line = cli.readStringUntil('\n');
    Serial.println(line);
  }
}
