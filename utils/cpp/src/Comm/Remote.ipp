#include "Comm/Remote.hpp"

Comm::Remote::Remote(Comm::Hub<std::string>* hub, std::string name)
  : hub(hub),
    name(name) {}

template<typename U>
void Comm::Remote::attach(std::string method, void (U::*handler) (std::vector<std::string>), U* _this){
  this->hub->on(this->name + '/' + method, std::bind(handler, _this, std::placeholders::_1));
}

void Comm::Remote::back(std::string method, std::vector<std::string> message){
  this->hub->emit(this->name + '/' + method + "/r", message);
}
