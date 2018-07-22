#ifndef SUBSYSTEM_PID
#define SUBSYSTEM_PID

#include <Comm/Hub.hpp>
#include <Comm/Remote.hpp>
#include <PID/Controller.hpp>

namespace Subsystem{
  class PID;
}

class Subsystem::PID : public Comm::Remote{
public:
  PID(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name);
  void correct(std::vector<std::string> arguments);
  void pid(std::vector<std::string> arguments);
  void setpoint(std::vector<std::string> arguments);

private:
  Comm::Hub<int>* arduino;
  int handler;
  PID::Controller* pidController;
};

#endif
