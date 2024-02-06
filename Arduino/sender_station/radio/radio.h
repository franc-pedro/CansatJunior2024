#include <SPIFlash.h>
#include <RFM69.h>
#include <RFM69_ATC.h>

#ifndef MIN
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#endif

#define RADIO_SS_PIN 10
#define RADIO_IRQ_PIN 3
#define RADIO_NET_ID 100 // 0-255, must be the same on all nodes
#define RADIO_NODE_ID 2 // 0-254, must be unique in network, 255=broadcast
#define RADIO_GROUNDSTATION_NODE_ID 1 // same as above, the ground station
#define RADIO_ATC_RSSI -80
#define RADIO_MAX_BUFFER_SIZE 61 // library limitation
class StreamedRFM : public Print {
  RFM69_ATC _radio = RFM69_ATC(RADIO_SS_PIN, RADIO_IRQ_PIN);
  uint8_t _buffer[RADIO_MAX_BUFFER_SIZE] = {0};
  uint8_t _buffer_len = 0;

public:
  void setup() {
    // Radio
    _radio.initialize(RF69_433MHZ, RADIO_NODE_ID, RADIO_NET_ID);
    _radio.setHighPower();
    _radio.encrypt(null);
    _radio.enableAutoPower(RADIO_ATC_RSSI);
  }

  void setFrequency(uint32_t freq) {
    _radio.setFrequency(freq);
  }

  int buffer_space() {
    return RADIO_MAX_BUFFER_SIZE - _buffer_len;
  }

  void flush() {
    if (_buffer_len == 0) {
      return;
    }

    Serial.print("saída rádio: ");
    Serial.write(_buffer, _buffer_len);
    Serial.println();
    _radio.send(RADIO_GROUNDSTATION_NODE_ID, _buffer, _buffer_len);
    _buffer_len = 0;
  }

  size_t write(uint8_t const* payload, int len) {
    int i = 0;
    while (i < len) {
      if (buffer_space() == 0) {
        flush();
      }

      int sz = MIN(buffer_space(), len - i);
      memcpy(_buffer+_buffer_len, payload+i, sz);
      i += sz;
      _buffer_len += sz;
    }

    return len;
  }

  size_t write(uint8_t c) {
    return StreamedRFM::write(&c, 1);
  }

  int availableForWrite() {
    return buffer_space();
  }

  // extra decimal places for floating point
  // the next definitions will shadow Print::print and Print::println, bring them to the derived class
  using Print::print;
  using Print::println;

  size_t print(double d, int n = 5) {
    return Print::print(d, n);
  }

  size_t println(double d, int n = 5) {
    return Print::println(d, n);
  }

  size_t print(float f, int n = 5) {
    return Print::print(f, n);
  }

  size_t println(float f, int n = 5) {
    return Print::println(f, n);
  }
} radio;