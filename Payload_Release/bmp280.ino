#include <Adafruit_BMP280.h>
#include <ESP32Servo.h>

Adafruit_BMP280 bmp;  // I2C Interface
Servo myservo;

#define PNM 1023.50  //Pressão média nível do mar do dia
#define LARGADA 1    // altitude da largada em metros
#define DEBUG 1      //

int pos = 0;  // variavel para a posiçaõ do servo
int servoPin = 26;
float firstAlt = 0;  // variável da primeira medida de altitude
float altitude = 0;
unsigned long previousMillis = 0;
const long interval = 500;
bool status = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor,  check wiring!"));
    while (1)
      ;
  }

  /* Default settings from datasheet.  */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400);  // attaches the servo on pin 18 to the servo object
                                        // using default min/max of 1000us and 2000us
                                        // different servos may require different min/max settings
                                        // for an accurate 0 to 180 sweep
  firstAlt = bmp.readAltitude(PNM);
  Serial.println(firstAlt);
}

void loop() {
  unsigned long currentMillis = millis();

#ifdef DEBUG
  Serial.print(F("Temperature  = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure() / 100);  //displaying the Pressure in hPa, you can change the unit
  Serial.println("  hPa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(PNM));  //The "1019.66" is the pressure(hPa) at sea level in day in your region
  Serial.println("  m");                //If you don't know it, modify it until you get your current  altitude
  Serial.println();
  delay(2000);
#endif

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    altitude = bmp.readAltitude(PNM);
  }
  if (status == 0) {
    if (altitude - firstAlt >= LARGADA) {
      for (pos = 0; pos <= 90; pos += 5) {
        myservo.write(pos);  // tell servo to go to position in variable 'pos'
        delay(30);           // waits 15ms for the servo to reach the position        
      }
      myservo.detach();
      status = 1;
    }
  }
}
