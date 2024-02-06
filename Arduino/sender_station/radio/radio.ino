#include "radio.h"

void setup() {
  Serial.begin(115200);
  radio.setup();
}

void loop() {
  radio.setFrequency(433000000);
  radio.println("Hello");
  radio.flush();
}
