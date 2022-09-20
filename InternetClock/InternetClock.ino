#include<ESP8266WiFi.h>
#include<WiFiUdp.h> //Present in ESP8266WiFi lib but need to write
#include<Wire.h> //For I2C Oled
#include<Adafruit_SSD1306.h>
#include<NTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define NTP_OFFSET 19800 // As indian GMT = +5:30 -> written in seconds
#define NTP_INTERVAL 60000 // In milli seconds
#define NTP_ADDRESS "0.in.pool.ntp.org"
String ssid = "WIFINAME";
String pass = "WIFIPASS";

WiFiUDP udp;
NTPClient timeClient(udp, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting To WiFi");
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi Connected Successfully");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to OLED");
  while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("Connection to OLED Successful");
  display.clearDisplay();
  display.setTextColor(WHITE);

  timeClient.begin();

  display.setTextSize(2);
  display.setCursor(0,15);
  display.println("INTERNET");
  display.println("CLOCK");
  display.display();
  delay(3000);
  display.clearDisplay();
}

void loop() {
  timeClient.update(); //Gives updated time
  int hr = timeClient.getHours();

  //Comment This Section to Get 24 Hours Time
  String a_p = "AM";
  if(hr>12){
    hr = hr-12;
    a_p = "PM";
  }
  else{
    a_p = "AM";
  }
  
  int mi = timeClient.getMinutes();
  int se = timeClient.getSeconds();
  String hrs = String(hr);
  String mis = String(mi);
  String ses = String(se);
  if(hr<10){
    hrs = "0" + hrs;
  }
  if(mi<10){
    mis = "0" + mis;
  }
  if(se<10){
    ses = "0" + ses;
  }
  Serial.println(hrs + ":" + mis + ":" + ses);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print(hrs + ":" + mis + ":" + ses);
  
  //Comment This Statement to Get 24 Hrs Time
  display.setTextSize(1);
  display.print(" "+a_p);
  
  display.display();
  delay(200);
}
