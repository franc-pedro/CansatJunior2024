#include <Wire.h>
#include <Adafruit_BMP085.h>
class Pressure {
  Adafruit_BMP085 _bmp;

public:
  void setup() {
    if (!_bmp.begin()) {
      Serial.println("Sensor de pressão não encontrado. Verifica as tuas ligações.");
    }
  }

  double read() {
    _bmp.readTemperature(); // won't work without it, TODO: check if library takes care of this for us
    return _bmp.readPressure();
  }

  double altitude(float slp){
    _bmp.readAltitude(slp);
  }
} pressure;