#ifndef SUBSYSTEM_DIVE
#define SUBSYSTEM_DIVE

#include <Comm/Hub.hpp>
#include <Comm/Remote.hpp>

namespace Subsystem{
  class Dive;
}

class Subsystem::Dive : public Comm::Remote{
public:
  Dive(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name);
  void power(std::vector<std::string> arguments);

private:
  Comm::Hub<int>* arduino;
  int handler;
};

#endif
