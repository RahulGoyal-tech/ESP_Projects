/* Project - Rahul Goyal
 *  Open Maps Weather API to get: 
 *  1) Area Temperature - feels like x
 *  2) Pressure x
 *  3) Weather Description x
 *  4) Humidity x
 *  5) Wind speed x
 *  6) Cloud Percent x 
 *  7) All air pollution components + AQI x -> Take long and lat data from Weather response
 *  DHT Sensor:
 *  1) Temperature x
 *  2) Humidity x
 *  BMP280
 *  1) Altitude x
 *  2) Pressure x
 *  3) Water Boiling Point x
 *  Telegram Bot Integration with commands
 *  1) Weatherx
 *  2) Room x
 *  3) Air x
 *  4) Humidity x
 *  5) Altitude x
 *  6) Boil x
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <CTBot.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>

// Credentials
String ssid = "WIFINAME";
String pass = "WIFIPASS";

String WeatherAPI = "API_KEY";
String token = "API_TOKEN";

String OldZip = "ZIP";
String Zip = "ZIP";
String CountryCode = "IN";
// Comparing OldZip and Zip to update Country Code

//Global Variables to Store Data
String jsonData = "";
String jsonAir = "";
String ATemp = "";
String AHum = "";
String APress = "";
String AWeather = "";
String AWSpeed = "";
String ACloud = "";
String AQI = "";
String AAir[] = {"","","","","","","",""};
String longi = "";
String lati = "";

String DTemp = "";
String DHum = "";

String BAlt = "";
String BBoil = "";
String BPress = "";

unsigned long lastTime = 0;
unsigned long Timer = 10000; // 10 Second delay before each request

//Sensor Objects
#define DHTPIN D5
#define DHTTYPE DHT22
uint64_t TeleID = 851093119;
Adafruit_BMP280 bmp;
DHT dht(DHTPIN, DHTTYPE);
CTBot Tele;
TBMessage msg;
void setup() {
  Serial.begin(115200);

  //WIFI SETUP
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  //DHT SETUP
  dht.begin();

  //BMP SETUP
  if(!bmp.begin()){
    Serial.println("No Valid Sensor found");
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
  Adafruit_BMP280::SAMPLING_X2,
  Adafruit_BMP280::SAMPLING_X16,
  Adafruit_BMP280::FILTER_X16,
  Adafruit_BMP280::STANDBY_MS_500);

  // TELEGRAM SETUP
  Tele.setTelegramToken(token);
  if(Tele.testConnection()){
    Serial.println("Telegram Connection Successful");
  }
  else{
    Serial.println("Telegram Connection not Successful");
  }

  //Initial Readings
  Weather();
  delay(200);
  Quality();
  delay(200);
  DHTsens();
  delay(200);
  bmpsense();
  delay(200);
  
  Tele.sendMessage(851093119,"Forecast and Ambient Station Activated");
  Tele.sendMessage(851093119,"Commands: \nWeather -> Area Weather Conditions\nRoom -> Room Temperature, humidity and Pressure\nAir -> Air Quality\nAltitude -> Current Altitude\nBoil -> Water Boiling Point");
}

void loop() {
  if(Tele.getNewMessage(msg)){
    if(msg.text.equalsIgnoreCase("Weather")){
      Weather();
      String temp = "Temperature: " + ATemp + "*C\nHumidity: " + AHum + "%\nPressure: " + APress + "hPa\nWeather Type: " + AWeather + "\nWind Speed: " + AWSpeed + "m\\s\nAmount of Cloud: " + ACloud + "%"; 
      Tele.sendMessage(msg.sender.id,temp);
    }
    if(msg.text.equalsIgnoreCase("Room")){
      DHTsens();
      bmpsense();
      String room = "Temperature: " + DTemp + "*C\nHumidity: " + DHum + "%\nPressure: " + BPress + "hPa";
      Tele.sendMessage(msg.sender.id,room);
    }
    if(msg.text.equalsIgnoreCase("Air")){
      Quality();
      if (AQI == "1"){
        String aqi = "AQI: " + AQI + " -> Excellent"; 
        Tele.sendMessage(msg.sender.id,aqi);
      }
      else if (AQI == "2"){
        String aqi = "AQI: " + AQI + " -> Good"; 
        Tele.sendMessage(msg.sender.id,aqi);
      }
      else if (AQI == "3"){
        String aqi = "AQI: " + AQI + " -> Ok"; 
        Tele.sendMessage(msg.sender.id,aqi);
      }
      else if (AQI == "4"){
        String aqi = "AQI: " + AQI + " -> Bad"; 
        Tele.sendMessage(msg.sender.id,aqi);
      }
      else if (AQI == "5"){
        String aqi = "AQI: " + AQI + " -> Poor"; 
        Tele.sendMessage(msg.sender.id,aqi);
      }
      String air = "Composition of Air: \nCO: " + AAir[0] + "ug/m3\nNO: " + AAir[1] + "ug/m3\nNO2: " + AAir[2] + "ug/m3\nO3: " + AAir[3] + "ug/m3\nSO2: " + AAir[4] + "ug/m3\nPM 2.5: " + AAir[5] + "ug/m3\nPM 10: " + AAir[6] + "ug/m3\nNH3: " + AAir[7] + "ug/m3";
      Tele.sendMessage(msg.sender.id,air);
    }
    if(msg.text.equalsIgnoreCase("Altitude")){
      bmpsense();
      String alt = "Altitude: " + BAlt + "m"; 
      Tele.sendMessage(msg.sender.id,alt);
    } 
    if(msg.text.equalsIgnoreCase("Boil")){
      bmpsense();
      String boil = "Water Boiling Point at Current Location: " + BBoil + "*F"; 
      Tele.sendMessage(msg.sender.id,boil);
    }
  }
}

void Weather(){
  
  DynamicJsonDocument Data(1024);
  String serverPath = "http://api.openweathermap.org/data/2.5/weather?zip=" + Zip + "," + CountryCode + "&appid=" + WeatherAPI;
  jsonData = Request(serverPath.c_str());
  Serial.println(jsonData);
  deserializeJson(Data, jsonData);
  
  String longit = Data["coord"]["lon"];
  String latit = Data["coord"]["lat"];
  longi = longit;
  lati = latit;
  Serial.print("Temperature: ");
  String temper = Data["main"]["temp"];
  ATemp = String(temper.toFloat()-273.15);
  Serial.println(ATemp);
  Serial.print("Pressure: ");
  String Press = Data["main"]["pressure"];
  APress = Press;
  Serial.println(APress);
  Serial.print("Humidity: ");
  String Hum = Data["main"]["humidity"];
  AHum = Hum;
  Serial.println(AHum);
  Serial.print("Wind Speed: ");
  String Wspeed = Data["wind"]["speed"];
  AWSpeed = Wspeed;
  Serial.println(AWSpeed);
  Serial.print("Weather Description: ");
  String WDesc = Data["weather"][0]["description"];
  AWeather = WDesc;
  Serial.println(AWeather);
  Serial.print("Cloud Precentage: ");
  String Cloud = Data["clouds"]["all"];
  ACloud = Cloud;
  Serial.println(ACloud);
}

void Quality(){
  
  jsonAir = "";
  DynamicJsonDocument Air(2048);
  
  Serial.println(longi);
  Serial.println(lati);
  Serial.println(Air.capacity());

  String serverAir = "http://api.openweathermap.org/data/2.5/air_pollution/forecast?lat=" + lati +"&lon=" + longi + "&appid=" + WeatherAPI;
  jsonAir = Request(serverAir.c_str());
  Serial.println(jsonAir);
  Serial.print("AQI: ");
  String OAQI = Air["list"][0]["main"]["aqi"];
  if (OAQI == "null"){
    Serial.println("Air Values Not Updated");
  }
  else{
  AQI = OAQI;
  String CO = Air["list"][1]["components"]["co"];
  String NO = Air["list"][1]["components"]["no"];
  String NO2 = Air["list"][1]["components"]["no2"];
  String O3 = Air["list"][1]["components"]["o3"];
  String SO2 = Air["list"][1]["components"]["so2"];
  String PM2 = Air["list"][1]["components"]["pm2_5"];
  String PM10 = Air["list"][1]["components"]["pm10"];
  String NH3 = Air["list"][1]["components"]["nh3"];
  AAir[0] = CO;
  AAir[1] = NO;
  AAir[2] = NO2;
  AAir[3] = O3;
  AAir[4] = SO2;
  AAir[5] = PM2;
  AAir[6] = PM10;
  AAir[7] = NH3;
  }
  Serial.println(AQI);

  Serial.println("Air Quality");
  for(int i=0; i<8; i++){
    Serial.println(AAir[i]);
  }
}

String Request(const char* serverName) {
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
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void DHTsens(){
  DTemp = dht.readTemperature();
  DHum = dht.readHumidity();
  Serial.println(DTemp);
  Serial.println(DHum);
}

void bmpsense(){
  BAlt = String(bmp.readAltitude(APress.toFloat()));
  BPress = String(bmp.readPressure());
  BBoil = String(bmp.waterBoilingPoint(bmp.readPressure()));
  Serial.println(BAlt);
  Serial.println(BPress);
  Serial.println(BBoil);
}
