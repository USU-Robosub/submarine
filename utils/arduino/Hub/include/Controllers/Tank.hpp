#ifndef CONTROLLERS_TANK
#define CONTROLLERS_TANK

#include <FreezableController.hpp>

#include <Arduino.h>

#ifndef TEENSY
  #include <Servo.h>
#else // teensy boards use a different library for servos
  #include <PWMServo.h>
  #define Servo PWMServo
#endif

namespace Controllers{
  class Tank;
}

class Controllers::Tank : public FreezableController{
public:
  Tank(int leftPin, int rightPin, bool protectMotors = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();

private:
  Servo left;
  Servo right;
  bool protectMotors;
  bool stopped;
  int leftPin;
  int rightPin;
};

#endif
