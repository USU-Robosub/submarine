#ifndef CONTROLLERS_TANK
#define CONTROLLERS_TANK

#include <FreezableController.hpp>
#include <Components/Motors/Motor.hpp>

#include <Arduino.h>

namespace Controllers{
  class Tank;
}

class Controllers::Tank : public FreezableController{
public:
  Tank(Components::Motors::Motor* left, Components::Motors::Motor* right);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();

private:
  Components::Motors::Motor* leftMotor;
  Components::Motors::Motor* rightMotor;
};

#endif
