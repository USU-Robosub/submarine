#ifndef CONTROLLERS_DIVE
#define CONTROLLERS_DIVE

#include <FreezableController.hpp>

#include <Arduino.h>

#ifndef TEENSY
  #include <Servo.h>
#else // teensy boards use a different library for servos
  #include <PWMServo.h>
  #define Servo PWMServo
#endif

namespace Controllers{
  class Dive;
}

class Controllers::Dive : public FreezableController{
public:
  Dive(int frontPin, int backPin, bool protectMotors = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void freeze();
  void unfreeze();

private:
  Servo front;
  Servo back;
  int frontPin;
  int backPin;
  bool protectMotors;
  bool stopped;
};

#endif
