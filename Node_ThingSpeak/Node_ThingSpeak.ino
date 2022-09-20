#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;

#define mic D5

long chan = 1453408;
const char WAPI[]="apikey";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin("WIFINAME","WIFIPASS");
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print("..");
    delay(200);
  }
  Serial.println();
  Serial.println("Node MCU Started");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
  pinMode(mic, INPUT);
}

void loop()
{
  float m = analogRead(mic);
  Serial.println("Value: " + (String) m);
  ThingSpeak.writeField(chan,1,m,WAPI);
  delay(100);
}
