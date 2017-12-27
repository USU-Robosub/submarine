#ifndef COMM_REMOTE
#define COMM_REMOTE

#include <Comm/Hub.hpp>

namespace Comm{
  class Remote;
}

class Comm::Remote{
public:
  Remote(Comm::Hub<std::string>* hub, std::string name);
  template<typename U>
  void attach(std::string method, void (U::*handler) (std::vector<std::string>), U* _this);
  void back(std::string method, std::vector<std::string> message);

protected:
  Comm::Hub<std::string>* hub;
  std::string name;
};

#include "../../src/Comm/Remote.ipp"

#endif
