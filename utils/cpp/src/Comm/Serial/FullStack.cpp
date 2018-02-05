#include <Comm/Serial/FullStack.hpp>

Comm::Serial::FullStack::FullStack(std::string portName, speed_t speed)
  : port(portName, speed),
    stream(&this->port),
    bridge(&this->stream),
    _hub(&this->bridge) {}

Comm::Hub<int>* Comm::Serial::FullStack::hub(){
  return &this->_hub;
}

void Comm::Serial::FullStack::restartArduino(){
  this->port.restartArduino();
}
