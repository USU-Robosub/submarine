#include <Subsystem/Dive.hpp>
#include <Comm/tools.hpp>

Subsystem::Dive::Dive(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name)
  : Comm::Remote(agent, name),
    arduino(arduino),
    handler(handler){
  this->attach("power", &Subsystem::Dive::power, this);
}

void Subsystem::Dive::power(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    this->arduino->emit(this->handler, {Comm::floatAsInt32(amount)});
  }
}
