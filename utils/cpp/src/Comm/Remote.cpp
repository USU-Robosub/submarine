#include "Comm/Remote.hpp"

Comm::Remote::Remote(Comm::Hub<std::string>* hub, std::string name)
  : hub(hub),
    name(name) {}

void Comm::Remote::back(std::string method, std::vector<std::string> message){
  this->hub->emit(this->name + '/' + method + "/r", message);
}
