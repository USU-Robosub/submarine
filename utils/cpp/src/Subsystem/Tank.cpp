#include <Subsystem/Tank.hpp>
#include <Comm/tools.hpp>

Subsystem::Tank::Tank(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name)
  : Comm::Remote(agent, name),
    arduino(arduino),
    handler(handler),
    throttlePower(0),
    steeringPower(0){
  this->attach("throttle", &Subsystem::Tank::throttle, this);
  this->attach("steering", &Subsystem::Tank::steering, this);
  this->attach("left", &Subsystem::Tank::left, this);
  this->attach("right", &Subsystem::Tank::right, this);
}

void Subsystem::Tank::throttle(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    this->throttlePower = amount;
    this->update();
  }
}

void Subsystem::Tank::steering(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    this->steeringPower = amount;
    this->update();
  }
}

/*
left = throttle + steering
right = throttle - steering

left + right = throttle + throttle + steering - steering
left + right = 2 * throttle
left - right = throttle + steering - throttle + steering
left - right = 2 * steering
*/
void Subsystem::Tank::left(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float left = std::stof(arguments[0]);
    float oldRight = this->throttlePower - this->steeringPower;
    this->throttlePower = (left + oldRight) / 2.0f;
    this->steeringPower = (left - oldRight) / 2.0f;
    this->update();
  }
}

void Subsystem::Tank::right(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float right = std::stof(arguments[0]);
    float oldLeft = this->throttlePower + this->steeringPower;
    this->throttlePower = (oldLeft + right) / 2.0f;
    this->steeringPower = (oldLeft - right) / 2.0f;
    this->update();
  }
}

void Subsystem::Tank::update(){
  this->arduino->emit(this->handler, {Comm::floatAsInt32(this->throttlePower), Comm::floatAsInt32(this->steeringPower)});
}
