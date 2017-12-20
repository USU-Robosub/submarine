#ifndef COMM_SERIAL_BRIDGE
#define COMM_SERIAL_BRIDGE

#include <Comm/Bridge.h>
#include <Comm/Serial/IntStreamWrapper.hpp>

namespace Comm{
  namespace Serial{
    class Bridge;
  }
}

class Comm::Serial::Bridge : public Comm::Bridge<int>{
public:
  Bridge(Comm::IntStreamWrapper* stream);
  void send(std::vector<int> data);
  std::queue<std::vector<int>> receive();

private:
  Comm::IntStreamWrapper* stream;
};

#endif
