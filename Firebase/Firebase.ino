#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin("WIFINAME", "WIFIPASS");
  Serial.println("Connecting To WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("#");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected. IP Address: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin("FIREBASEHOST", "FIREBASEAUTH");
}


void loop() {
  // put your main code here, to run repeatedly:
  Firebase.setInt("Node/value",1);
  delay(1000);
  Firebase.setInt("Node/value",0);
  delay(10000);
}
