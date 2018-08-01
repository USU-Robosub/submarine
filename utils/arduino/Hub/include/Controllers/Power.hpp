#ifndef CONTROLLERS_POWER
#define CONTROLLERS_POWER

#include <FreezableController.hpp>

#include <Arduino.h>

namespace Controllers{
  class Power;
}

class Controllers::Power : public FreezableController{
public:
  Power(unsigned int pin);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();

private:
  unsigned int pin;
};

#endif
