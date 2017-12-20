#ifndef COMM_SERIAL_BRIDGE
#define COMM_SERIAL_BRIDGE

#include <Comm/Bridge.h>
#include <Comm/Stream.hpp>

namespace Comm{
  namespace Serial{
    class Bridge;
  }
}

class Comm::Serial::Bridge : public Comm::Bridge<int>{
public:
  Bridge(Comm::Stream<int>* stream);
  void send(std::vector<int> data);
  std::queue<std::vector<int>> receive();

private:
  Comm::Stream<int>* stream;
};

#endif
