#ifndef CONTROLLERS_TANK
#define CONTROLLERS_TANK

#include <Controller.hpp>

#include <Arduino.h>

#ifndef UNIT_TESTS

  #ifndef TEENSY
    #include <Servo.h>
  #else // teensy boards use a different library for servos
    #include <PWMServo.h>
    #define Servo PWMServo
  #endif

#else
  #include <Servo.mock.hpp>
#endif

namespace Controllers{
  class Tank;
}

class Controllers::Tank : public Controller{
public:
  Tank(int leftPin, int rightPin, bool protectMotors = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void kill();

private:
  Servo left;
  Servo right;
  bool protectMotors;
  bool stopped = false;
  int leftPin;
  int rightPin;
};

#endif
