#include <Subsystem/Power.hpp>
#include <Comm/tools.hpp>

Subsystem::Power::Power(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name)
  : Comm::Remote(agent, name),
    arduino(arduino),
    handler(handler){
  this->attach("enable", &Subsystem::Power::enable, this);
}

void Subsystem::Power::enable(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stoi(arguments[0]);
    if(amount == 0 || amount == 1) {
        this->arduino->emit(this->handler, {amount});
    }
  }
}