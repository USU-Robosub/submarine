#ifndef COMM_TCP_BRIDGE
#define COMM_TCP_BRIDGE

#include <Comm/Bridge.hpp>
#include <Comm/Stream.hpp>
#include <string>

namespace Comm{
  namespace TCP{
    class Bridge;
  }
}

class Comm::TCP::Bridge : public Comm::Bridge<std::string>{
public:
  Bridge(Comm::Stream<std::string>* stream);
  void send(std::vector<std::string> data);
  std::queue<std::vector<std::string>> receive();

private:
  Comm::Stream<std::string>* stream;
};

#endif
