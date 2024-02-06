#include "temperature.h"

long int temperatura;


void setup() {
  Serial.begin(9600);
  temperature.setup();
}

void loop() {
  temperature.requestTemperatures();
  temperatura = temperature.getTemperatureForIndex(0);
  Serial.println(temperatura);
  delay(500);
}
