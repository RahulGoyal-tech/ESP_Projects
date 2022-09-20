#include "CTBot.h"

CTBot myBot;

String ssid = "WIFINAME";
String pass = "WIFIPASS";

String token = "TELEGRAM_TOKEN";

uint8_t greenled = D6;
uint8_t redled = D1;
uint8_t whiteled = D2;

void setup() {

myBot.wifiConnect(ssid, pass);

myBot.setTelegramToken(token);

if (myBot.testConnection()){
  Serial.println("\ntestConnection OK");
else{}

  Serial.println("\ntestConnection NOK");
}

pinMode(redled, OUTPUT);
digitalWrite(redled, LOW);
pinMode(greenled, OUTPUT);
digitalWrite(greenled, LOW);
pinMode(whiteled, OUTPUT);
digitalWrite(whiteled, LOW);
}
void loop() {

TBMessage msg;
if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("RED LIGHT ON")) {
    digitalWrite(redled, HIGH);                
    myBot.sendMessage(msg.sender.id, "RED Light is now ON");
    }
    
    else if (msg.text.equalsIgnoreCase("RED LIGHT OFF")) {
    digitalWrite(redled, LOW);
    myBot.sendMessage(msg.sender.id, "RED Light is now OFF");
    }
    
    else if (msg.text.equalsIgnoreCase("GREEN LIGHT ON")){
    digitalWrite(greenled, HIGH);                
    myBot.sendMessage(msg.sender.id, "GREEN Light is now ON");
    }
    
    else if (msg.text.equalsIgnoreCase("GREEN LIGHT OFF")){
    digitalWrite(greenled, LOW);               // turn off the led
    myBot.sendMessage(msg.sender.id, "GREEN Light is now OFF"); // notify the sender
    }
    
    else if (msg.text.equalsIgnoreCase("White LIGHT ON")){
    digitalWrite(whiteled, HIGH);                // turn on the LED
    myBot.sendMessage(msg.sender.id, "WHITE Light is now ON"); // notify the sender
    }
    
    else if (msg.text.equalsIgnoreCase("WHITE LIGHT OFF")){
    digitalWrite(whiteled, LOW);               // turn off the led
    myBot.sendMessage(msg.sender.id, "WHITE Light is now OFF"); // notify the sender
    }
  
    else if (msg.text.equalsIgnoreCase("All Off")){
      digitalWrite(whiteled, LOW); 
      digitalWrite(redled, LOW); 
      digitalWrite(greenled, LOW); 
      myBot.sendMessage(msg.sender.id, "ALL LIGHTS ARE OFF"); // notify the sender
    }
  
    else if (msg.text.equalsIgnoreCase("All On")){
      digitalWrite(whiteled, HIGH); 
      digitalWrite(redled, HIGH); 
      digitalWrite(greenled, HIGH); 
      myBot.sendMessage(msg.sender.id, "ALL LIGHTS ARE ON"); // notify the sender
    }
  }
}
