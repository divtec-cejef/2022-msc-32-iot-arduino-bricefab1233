#include "DHT.h"
#include "timer.h"
#include <SigFox.h>
#include <ArduinoLowPower.h>
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Timer timer;

const int BUTTON_PIN = 1;
int previousButtonState = 0;

float humidity = 0;
float temperature = 0;

/**
 * Affcihe les températures dans la console,
 * Envoie les données de températures et humidité à SigFox
 */
void showTemperature() {

  //Initialise la variable à l'humidité capté par le capteur
  humidity = dht.readHumidity();
  //Initialise la variable à la température capté par le capteur
  temperature = dht.readTemperature();
  temperature -= 1;
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C \n");

  SigFox.begin();
  SigFox.beginPacket();
  SigFox.write(humidity);
  SigFox.write(temperature);
  SigFox.endPacket();
  SigFox.end();
}

void setup() {
  Serial.begin(9600);

  //Initialisation du timer toutes les 15 minutes 
  timer.setInterval(9000000);
  timer.setCallback(showTemperature);
  timer.start();

  // initialise le bouton à une entrée
  pinMode(BUTTON_PIN, INPUT);

  dht.begin();
}

void loop() {
  bool currentButtonState = digitalRead(BUTTON_PIN);
  //Vérifie que le précédente valeur du bouton soit off et que la valeur actuel du bouton soit on
  if (previousButtonState != currentButtonState && currentButtonState == HIGH) {
    showTemperature();
  }
  previousButtonState = currentButtonState;

  timer.update(); // update the timer
}
