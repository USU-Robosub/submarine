#ifndef COMPONENTS_MOTORS_MOTOR
#define COMPONENTS_MOTORS_MOTOR

#define MOTOR_REVERSE -1
#define MOTOR_STOP 0
#define MOTOR_FORWARD 1

namespace Components{
  namespace Motors{
    class Motor;
  }
}

class Components::Motors::Motor {
public:
  virtual void power(float amount) = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;
};

#endif
