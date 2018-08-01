#ifndef SUBSYSTEM_POWER
#define SUBSYSTEM_POWER

#include <Comm/Hub.hpp>
#include <Comm/Remote.hpp>

namespace Subsystem{
  class Power;
}

class Subsystem::Power : public Comm::Remote{
public:
  Power(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name);
  void enable(std::vector<std::string> arguments);

private:
  void update();
  
  Comm::Hub<int>* arduino;
  int handler;
};

#endif
