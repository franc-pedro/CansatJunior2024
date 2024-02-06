#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef MIN
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#endif
#define TEMPERATURE_PIN 4
#define DS18B20_MAX_CONVERSION_TIMEOUT 750 /* from library */

void xdelay(unsigned long d) {
  unsigned long startTime = millis();
  if (millis() < startTime + d) {
    delay(MIN(d, millis() - startTime - d));
  }
}

class Temperature {
private:
  OneWire _bus = OneWire(TEMPERATURE_PIN);
  DallasTemperature _sensors = DallasTemperature(&_bus);
  unsigned long _lastReq = 0;

  void _blockTillConversionComplete() {
    if (_lastReq == 0) { // equivalent to completed conversion
      return;
    }

    while (!_sensors.isConversionComplete() && (millis() - _lastReq < DS18B20_MAX_CONVERSION_TIMEOUT)) {
      xdelay(MIN(1, DS18B20_MAX_CONVERSION_TIMEOUT - (millis() - _lastReq)));
    }

    _lastReq = 0;
  }

public:
  void setup() {
    _sensors.begin();
    _sensors.setWaitForConversion(false);
    _sensors.setResolution(9);

    Serial.print("Detetados ");
    Serial.print(_sensors.getDeviceCount());
    Serial.println(" sensores de temperatura DS18B20");
  }

  void setResolution(uint8_t res) {
    _sensors.setResolution(res);
  }

  void requestTemperatures() {
    _sensors.requestTemperatures();
    _lastReq = millis();
  }

  double getTemperatureForIndex(uint8_t idx) {
    _blockTillConversionComplete();
    return _sensors.getTempCByIndex(idx);
  }
} temperature;