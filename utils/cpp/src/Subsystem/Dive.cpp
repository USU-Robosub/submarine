#include <Subsystem/Dive.hpp>
#include <Comm/tools.hpp>

Subsystem::Dive::Dive(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name)
  : Comm::Remote(agent, name),
    arduino(arduino),
    handler(handler),
    divePower(0),
    steeringPower(0){
  this->attach("power", &Subsystem::Dive::power, this);
  this->attach("steering", &Subsystem::Dive::steering, this);
}

void Subsystem::Dive::power(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    this->divePower = amount;
    this->update();
  }
}

void Subsystem::Dive::steering(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    this->steeringPower = amount;
    this->update();
  }
}

void Subsystem::Dive::update(){
  this->arduino->emit(this->handler, {Comm::floatAsInt32(this->divePower), Comm::floatAsInt32(this->steeringPower)});
}
