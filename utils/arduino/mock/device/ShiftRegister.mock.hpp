#ifndef MOCK_DEVICE_SHIFT_REGISTER
#define MOCK_DEVICE_SHIFT_REGISTER

#define SHIFT_REGISTER_PIN_COUNT 8

#include <Arduino.mock.hpp>
#include <functional>

namespace Mock{
  namespace Device{
    class ShiftRegister;
  }
}

template<class T, class Method, Method m, class ...Params>
static auto bounce(void *priv, Params... params) ->
		decltype(((*reinterpret_cast<T *>(priv)).*m)(params...))
{
	return ((*reinterpret_cast<T *>(priv)).*m)(params...);
}

#define BOUNCE(c,m) bounce<c, decltype(&c::m), &c::m>

class Mock::Device::ShiftRegister{
public:
  ShiftRegister(int shiftClockPin, int storageClockPin, int dataPin)
    : shiftClockPin(shiftClockPin),
      storageClockPin(storageClockPin),
      dataPin(dataPin){
    Mock::Arduino::$listenToDigitalPin(shiftClockPin, &BOUNCE(ShiftRegister, shiftClockPinChanged), this);
    Mock::Arduino::$listenToDigitalPin(storageClockPin, &BOUNCE(ShiftRegister, storageClockPinChanged), this);
  }

  void shiftClockPinChanged(){
    if(Mock::Arduino::digitalPinStates[this->shiftClockPin] == HIGH){
      for(int i = SHIFT_REGISTER_PIN_COUNT - 2; i >= 0; i--){ // shift data
        this->storage[i + 1] = this->storage[i];
      }
      this->storage[0] = Mock::Arduino::digitalPinStates[this->dataPin];
    }
  }

  void storageClockPinChanged(){
    if(Mock::Arduino::digitalPinStates[this->storageClockPin] == HIGH){
      for(int i = 0; i < SHIFT_REGISTER_PIN_COUNT; i++){
        this->pins[i] = this->storage[i];
      }
    }
  }

  int storage[SHIFT_REGISTER_PIN_COUNT] = {LOW};
  int pins[SHIFT_REGISTER_PIN_COUNT] = {NOT_SET};

private:
  int shiftClockPin;
  int storageClockPin;
  int dataPin;
};

#endif
