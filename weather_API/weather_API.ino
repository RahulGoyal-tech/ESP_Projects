#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <CTBot.h>

CTBot tele;

const char* ssid = "WIFINAME";
const char* password = "WIFIPASS";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "OPEN_WEATHER_API_KEY";
String token = "OPEN_WEATHER_API_TOKEN";
// Example:
//String openWeatherMapApiKey = "bd939aa3d23ff33d3c8f5dd1dd4";
//api.openweathermap.org/data/2.5/weather?zip={zip code},{country code}&appid={API key}
// Replace with your country code and city
String zip = "110001";
String countryCode = "IN";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

String temptele;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
  tele.setTelegramToken(token);
  if(tele.testConnection()){
    Serial.println("Telegram Connection Successful");
  }
  else{
    Serial.println("Telegram Connection not Successful");
  }
}

void loop() {
  // Send an HTTP GET request
  JSONVar myObject;
  TBMessage msg;
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?zip=" + zip + "," + countryCode + "&appid=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.println("");
      Serial.println("");
      String temper = JSON.stringify(myObject["main"]["temp"]);
      temptele = temper;
      Serial.println(temper);
      Serial.println("");
      Serial.println("");
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  if(tele.getNewMessage(msg)){
    if(msg.text.equalsIgnoreCase("Temperature")){
      //tele.sendMessage(msg.sender.id,temp);
      float tempC = temptele.toFloat() - 273.15;
      
      String temp = "Temperature is " + String(tempC) + "*C";
      tele.sendMessage(msg.sender.id,temp);
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
