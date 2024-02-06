#include "pressure.h"

#define SLP 102500  // Temos de consultar as cartas barométricas para o dia em causa
                    // Por exemplo - https://www.ipma.pt/pt/otempo/prev.numerica/index-mf2.js
long int pressao;
float altitude;

void setup() {
  Serial.begin(9600);
  pressure.setup();
}

void loop() {
  pressao = pressure.read(); // Lemos a pressão atmosférica
  Serial.println(pressao);
  delay(500);

  altitude = pressure.altitude(SLP);  // Lemos a altitude em função da pressão média ao nível do mar                                      
  Serial.println(altitude);
  delay(500);
}
