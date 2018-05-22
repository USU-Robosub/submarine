#include <Hub.hpp>
#include <Controllers/DC_MotorDriver.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/Dive.hpp>
#include <PinMap.hpp>

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


#define ECHO_RETURN 42

Hub* hub;
Controller** controllers;

void setup()
{
  controllers = new Controller*[2];
  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::DC_MotorDriver(LEFT_MOTOR_SPEED
                                                    , LEFT_MOTOR_FORWARD
                                                    , LEFT_MOTOR_BACKWARD
                                                    , RIGHT_MOTOR_SPEED
                                                    , RIGHT_MOTOR_FORWARD
                                                    , RIGHT_MOTOR_BACKWARD);
  hub = new Hub(controllers, 2);
}

void loop() {
  hub->poll();
  delay(1);
}
