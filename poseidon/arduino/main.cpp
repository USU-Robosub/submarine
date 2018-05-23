#include <Hub.hpp>
#include <Controllers/DC_MotorDriver.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
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

Hub* hub;
Controller** controllers;

void setup()
{
  controllers = new Controller*[2];
  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::KillSwitch(KILL_PIN, 1);
  controllers[2] = new Controllers::Dive(TEST_MOTOR_PIN, TEST_MOTOR_PIN);
  hub = new Hub(controllers, 3);
}

void loop() {
  hub->poll();
  delay(1);
}
