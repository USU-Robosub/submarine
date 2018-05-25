#ifndef MOCK_ARDUINO
#define MOCK_ARDUINO

#include <types.mock.hpp>
#include <math.mock.hpp>
#include <Serial.mock.hpp>

#define DIGITAL_PIN_COUNT 20
#define ANALOG_PIN_COUNT 20

namespace Mock{
  namespace Arduino{
    extern int digitalPinModes[];
    extern int digitalPinStates[];
    extern int analogPinStates[];
  }
}

void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
void analogWrite(int pin, int value);

#ifdef MOCK_INCLUDE_SOURCE

namespace Mock{
  namespace Arduino{
    int digitalPinModes[DIGITAL_PIN_COUNT] = { -1 };
    int digitalPinStates[DIGITAL_PIN_COUNT] = { -1 };
    int analogPinStates[ANALOG_PIN_COUNT] = { -1 };
  }
}

void pinMode(int pin, int mode){
  Mock::Arduino::digitalPinModes[pin] = mode;
}

void digitalWrite(int pin, int value){
  Mock::Arduino::digitalPinStates[pin] = value;
}

void analogWrite(int pin, int value){
  Mock::Arduino::analogPinStates[pin] = value;
}

#endif
#endif
