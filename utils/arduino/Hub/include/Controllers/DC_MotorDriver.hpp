#ifndef CONTROLLERS_DC_MOTOR_DRIVER
#define CONTROLLERS_DC_MOTOR_DRIVER

#include <FreezableController.hpp>

#include <Arduino.h>

namespace Controllers{
  class DC_MotorDriver;
}

class Controllers::DC_MotorDriver : public FreezableController{
public:
  DC_MotorDriver(int pinSpeedA, int pinForwardA, int pinBackwardA, int pinSpeedB, int pinForwardB, int pinBackwardB, bool protectMotors  = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();
  
private:
  void stop();

  int pinSpeedA;
  int pinForwardA;
  int pinBackwardA;
  int pinSpeedB;
  int pinForwardB;
  int pinBackwardB;

  bool protectMotors;
  bool stopped;
};

#endif
