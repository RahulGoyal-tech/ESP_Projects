#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define LED1 2
#define LED2 D5

AsyncWebServer server(80);

String ssid = "WIFINAME";          // Your WiFi SSID //Uncomment this for Node MCU to Wifi
String password = "WIFIPASS";   // Your WiFi Password //Uncomment this for Node MCU to Wifi

//String ssid = "Node"; //Uncomment this for Node MCU as AP
int f = 3;

void debug(){
  WebSerial.println("Debug Mode Started");
  while(f==1){
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    delay(500);
  }
  WebSerial.println("Debug Mode Ended");
  digitalWrite(LED2, LOW);
}

void work(){
  WebSerial.println("Work Mode Started");
  while(f==0){
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    delay(500);
  }
  WebSerial.println("Work Mode Ended");
  digitalWrite(LED1, HIGH);
}

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "Debug"){
    f = 1;
  }
  if (d=="Work"){
    f = 0;
  }
  if (d == "Normal"){
    f = 3;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  WiFi.mode(WIFI_STA);

  //Uncomment this for Node MCU to Wifi
  WiFi.begin(ssid, password);

  //Uncomment this for Node MCU to AP
  //WiFi.softAP(ssid);
  
  //Uncomment this for Node MCU to Wifi , Comment for Node MCU as AP
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  
  Serial.println("IP Address: ");
  
  //Uncomment this for Node MCU to Wifi
  Serial.println(WiFi.localIP());
  
  //Uncomment this for Node MCU to AP
  //Serial.println(WiFi.softAPIP());
  
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}

void loop() {
  if (f==0){
    work();
  }
  if (f==1){
    debug();
  }
  WebSerial.println("Hello!");
  delay(2000);
}
