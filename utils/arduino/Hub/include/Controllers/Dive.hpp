#ifndef CONTROLLERS_DIVE
#define CONTROLLERS_DIVE

#include <Controller.hpp>

#include <Arduino.h>

#ifndef UNIT_TESTS
  #include <Arduino.h>

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
  class Dive;
}

class Controllers::Dive : public Controller{
public:
  Dive(int frontPin, int backPin, bool protectMotors = true);
  void execute(Emitter* hub, int32_t* data, int32_t length);

private:
  Servo front;
  Servo back;
  bool protectMotors;
};

#endif
