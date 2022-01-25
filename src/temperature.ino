#include "DHT.h"
#include <arduino-timer.h>
#include <SigFox.h>
#include <ArduinoLowPower.h>
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int buttonPin = 1;
auto timer = timer_create_default();
int previousButtonState = 0;
float h = 0;
float t = 0;

bool temperatureAffichage(void *)
{  
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C \n");

  SigFox.begin();
  SigFox.beginPacket();
  SigFox.print(t);
  SigFox.print(h);
  SigFox.endPacket();
    
  return true;
}

void setup()
{
  timer.every(10000, temperatureAffichage);
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  dht.begin();
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();

  bool currentButtonState = digitalRead(buttonPin);
  if (previousButtonState != currentButtonState && currentButtonState == HIGH)
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C \n");
  }

  timer.tick(); // tick the timer
  previousButtonState = currentButtonState;
}
