#ifndef CONTROLLERS_DC_MOTOR_DRIVER
#define CONTROLLERS_DC_MOTOR_DRIVER

#include <Controller.hpp>

#include <Arduino.h>

namespace Controllers{
  class DC_MotorDriver;
}

class Controllers::DC_MotorDriver : public Controller{
public:
  DC_MotorDriver(int pinSpeedA, int pinForwardA, int pinBackwardA, int pinSpeedB, int pinForwardB, int pinBackwardB, bool protectMotors  = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);

private:
  void stop();

  int pinSpeedA;
  int pinForwardA;
  int pinBackwardA;
  int pinSpeedB;
  int pinForwardB;
  int pinBackwardB;

  bool protectMotors;
};

#endif
