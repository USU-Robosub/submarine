#include <Comm/TCP/FullStack.hpp>

Comm::TCP::FullStack::FullStack(std::string address, unsigned int portNum, char separator)
  : port(address, portNum),
    stream(&this->port, separator),
    bridge(&this->stream),
    _hub(&this->bridge) {}

Comm::TCP::FullStack::FullStack(unsigned int portNum, char separator)
  : server(new Comm::TCP::SingleClientServer(portNum)),
    port(server->waitForConnection()),
    stream(&this->port, separator),
    bridge(&this->stream),
    _hub(&this->bridge) {}

Comm::TCP::FullStack::FullStack(Comm::TCP::Port port, char separator)
  : port(std::move(port)),
    stream(&this->port, separator),
    bridge(&this->stream),
    _hub(&this->bridge) {}

Comm::TCP::FullStack::~FullStack(){
  if(this->server != nullptr)
    delete this->server;
}

Comm::Hub<std::string>* Comm::TCP::FullStack::hub(){
  return &this->_hub;
}
