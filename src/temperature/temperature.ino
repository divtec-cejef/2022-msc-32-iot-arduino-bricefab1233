#include "DHT.h"
#include <arduino-timer.h>
#include <SigFox.h>
#include <ArduinoLowPower.h>
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

auto timer = timer_create_default();

const int BUTTON_PIN = 1;
int previousButtonState = 0;

float humdity = 0;
float temperature = 0;

/**
 * @brief Affcihe les températures dans la console,
 * Envoie les données de températures et humidité à SigFox
 */
void showTemperature()
{
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C \n");

  SigFox.begin();
  SigFox.beginPacket();
  SigFox.write(t);
  SigFox.write(h);
  SigFox.endPacket();
  SigFox.end();
}

/**
 * @brief Fonction qui est exécuté par le timer
 */
bool timerFunction(void *)
{
  showTemperature();
  return true;
}

void setup()
{
  // Toutes les 15 minutes la fonction est exécuté
  timer.every(900000, timerFunction);
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  // initialise le bouton à une entrée
  pinMode(BUTTON_PIN, INPUT);

  dht.begin();
}

void loop()
{
  //Initialise la variable à l'humidité capté par le capteur
  humidity = dht.readHumidity();
  //Initialise la variable à la température capté par le capteur
  temperature = dht.readTemperature();

  bool currentButtonState = digitalRead(BUTTON_PIN);
  //Vérifie que le précédente valeur du bouton soit off et que la valeur actuel du bouton soit on
  if (previousButtonState != currentButtonState && currentButtonState == HIGH)
  {
    showTemperature();
  }

  timer.tick(); // tick the timer
  previousButtonState = currentButtonState;
}
