#ifndef MOCK_COMPONENTS_MOTORS_MOTOR
#define MOCK_COMPONENTS_MOTORS_MOTOR

#include <Components/Motors/Motor.hpp>

namespace Mock{
  namespace Components{
    namespace Motors{
      class Motor;
    }
  }
}

class Mock::Components::Motors::Motor : public ::Components::Motors::Motor {
public:
  void power(float amount){
    this->$power = amount;
  }

  void enable(){
    this->$isEnabled = true;
  }

  void disable(){
    this->$isEnabled = false;
  }

  float $power = 0;
  bool $isEnabled = false;
};

#endif
