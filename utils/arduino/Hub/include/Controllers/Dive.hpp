#ifndef CONTROLLERS_DIVE
#define CONTROLLERS_DIVE

#include <FreezableController.hpp>
#include <Components/Motors/Motor.hpp>

#include <Arduino.h>

namespace Controllers{
  class Dive;
}

class Controllers::Dive : public FreezableController{
public:
  Dive(Components::Motors::Motor* front, Components::Motors::Motor* back);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();

private:
  Components::Motors::Motor* frontMotor;
  Components::Motors::Motor* backMotor;
};

#endif
