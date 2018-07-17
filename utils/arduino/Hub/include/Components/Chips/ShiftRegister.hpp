#ifndef COMPONENTS_CHIPS_SHIFT_REGISTER
#define COMPONENTS_CHIPS_SHIFT_REGISTER

#include <Arduino.h>

#define SHIFT_REGISTER_PIN_COUNT 8

namespace Components{
  namespace Chips{
    class ShiftRegister;
    struct ShiftRegisterConfig;
  }
}

struct Components::Chips::ShiftRegisterConfig{
  int shiftClockPin;
  int storageClockPin;
  int dataPin;
  int blinkDelay;
};

class Components::Chips::ShiftRegister{
public:
  ShiftRegister(Components::Chips::ShiftRegisterConfig config);
  virtual void pin(int pin, bool state);
  virtual void blinkPin(int pin);
  virtual void update();

private:
  void shiftInBit(bool state);
  void show();

  Components::Chips::ShiftRegisterConfig config;
  bool oldPinStates[SHIFT_REGISTER_PIN_COUNT] = {true};
  bool pinStates[SHIFT_REGISTER_PIN_COUNT] = {false};

  unsigned long blinkLastMillis[SHIFT_REGISTER_PIN_COUNT] = {0};
  bool blinkEnabled[SHIFT_REGISTER_PIN_COUNT] = {false};
};

#endif
