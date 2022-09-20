#include <DHT.h>

#define DHTPIN D5
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

float humid = 0; 
float temp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  humid = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity Reading: ");
  Serial.println(humid);
  Serial.print("Temperature Reading: ");
  Serial.println(temp);
  delay(100);
}
