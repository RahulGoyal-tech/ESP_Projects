#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

//OLED Parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Global variables for dht
float oldh = 0;
float oldt = 0;
float t = 0;
float h = 0;

//OLED object with I2c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//DHT Parameters
#define DHTPIN 14     //D5
#define DHTTYPE DHT22

//DHT Object
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Serial monitor and dht start
  Serial.begin(9600);
  dht.begin();

  //Waiting for OLED to start
  while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Allocating Display");
  }

  //Clearing Display
  display.clearDisplay();

  //Setting Text Color
  display.setTextColor(WHITE);
}

void loop() {

  //read temperature and humidity
  t = dht.readTemperature();
  h = dht.readHumidity();

  //Verify temperature and humidity readings
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Comparision current reading with previous one and updating on OLED if there is a change
  if((t!=oldt)||(h!=oldh))
  {
    printout(t,h);
  }

  //Making Current reading as previous one
  oldt = t;
  oldh = h; 
}

//Funtion To display temperature and humidity on OLED
void printout(float t, float h){

  //clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %"); 

  //function to actually display on screen
  display.display();
}
