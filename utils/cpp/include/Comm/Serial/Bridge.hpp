#ifndef COMM_SERIAL_BRIDGE
#define COMM_SERIAL_BRIDGE

#include <Comm/Bridge.hpp>
#include <Comm/Stream.hpp>
#include <Comm/Port.hpp>

namespace Comm{
  namespace Serial{
    class Bridge;
  }
}

class Comm::Serial::Bridge : public Comm::Bridge<int>{
public:
  Bridge(Comm::Stream<int>* stream, unsigned int maxReads = 500);
  void send(std::vector<int> data);
  std::queue<std::vector<int>> receive();

private:
  Comm::Stream<int>* stream;
  Comm::MessageState state;
  Comm::Message<int> currentMessage;
  int dataLeft;
  unsigned int maxReads;
};

#endif
