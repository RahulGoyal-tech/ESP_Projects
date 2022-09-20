#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "AUTH_KEY"; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, "WIFINAME", "WIFIPASS");
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}
