#ifndef SUBSYSTEM_TANK
#define SUBSYSTEM_TANK

#include <Comm/Hub.hpp>
#include <Comm/Remote.hpp>

namespace Subsystem{
  class Tank;
}

class Subsystem::Tank : public Comm::Remote{
public:
  Tank(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name);
  void throttle(std::vector<std::string> arguments);
  void steering(std::vector<std::string> arguments);
  void left(std::vector<std::string> arguments);
  void right(std::vector<std::string> arguments);

private:
  void update();

  Comm::Hub<int>* arduino;
  int handler;
  float throttlePower;
  float steeringPower;
};

#endif
