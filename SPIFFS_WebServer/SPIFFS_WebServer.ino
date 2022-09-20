#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

String ssid = "WIFINAME";
String pass = "WIFIPASS";

#define ledPin 2

String ledState;

AsyncWebServer server(80);

String getTemp(){
  float Temp = bmp.readTemperature();
  Serial.println(Temp);
  return String(Temp);
}

String getHum(){
  float Hum = bmp.readAltitude();
  Serial.println(Hum);
  return String(Hum);
}

String getPres(){
  float Pres = bmp.readPressure()/100.0F;
  Serial.println(Pres);
  return String(Pres);
}

String allValue(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "OFF";
    }
    else{
      ledState = "ON";
    }
    Serial.println(ledState);
    return ledState;
  }

  else if(var == "TEMPERATURE"){
    return getTemp();
  }
  else if(var == "HUMIDITY"){
    return getHum();
  }
  else if(var == "PRESSURE"){
    return getPres();
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Set your Static IP address
  IPAddress local_IP(192, 168, 1, 107);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  
  if (!WiFi.config(local_IP, gateway,subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  if(!bmp.begin()){
    Serial.println("Could not find BMP");
    return;
  }

  if(!SPIFFS.begin()){
    Serial.println("SPIFFS cannot be started");
    return;
  }

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());

  server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, allValue);
    });

  server.on("/style.css",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
    });

  server.on("/on",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW); // IN Built LED
    request->send(SPIFFS, "/index.html", String(), false, allValue);
    });

  server.on("/off",HTTP_GET,[](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH); // IN Built LED
    request->send(SPIFFS, "/index.html", String(), false, allValue);
    });

  server.on("/temperature",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemp().c_str());
    });

  server.on("/humidity",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHum().c_str());
    });

  server.on("/pressure",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getPres().c_str());
    });

  server.begin();

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500);
 }
  

void loop(){
  
}
