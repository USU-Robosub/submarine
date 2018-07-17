#ifndef COMPONENTS_MOTORS_BLUE_ROBOTICS_R1_ESC
#define COMPONENTS_MOTORS_BLUE_ROBOTICS_R1_ESC

#include <Components/Motors/Motor.hpp>
#include <Arduino.h>
#include <Servo.h>

#define BR_R1_PWM_MOTOR_REVERSE 1100
#define BR_R1_PWM_MOTOR_STOP 1500
#define BR_R1_PWM_MOTOR_FORWARD 1900

namespace Components{
  namespace Motors{
    class BlueRoboticsR1Esc;
    struct BlueRoboticsR1EscConfig;
    struct Trim;
  }
}

struct Components::Motors::Trim{
  float maxReverse;
  float minReverse;
  float stop;
  float minForward;
  float maxForward;
};

struct Components::Motors::BlueRoboticsR1EscConfig{
  int pin;
  Trim trim;
};

class Components::Motors::BlueRoboticsR1Esc : public Components::Motors::Motor{
public:
  BlueRoboticsR1Esc(Components::Motors::BlueRoboticsR1EscConfig config);
  void power(float amount);
  void enable();
  void disable();

private:
  void stop();
  void forward(float amount);
  void reverse(float amount);

  int pin;
  Servo servo;
  float maxReversePower;
  float minReversePower;
  float stopPower;
  float minForwardPower;
  float maxForwardPower;
  bool isEnabled;
};

#endif
