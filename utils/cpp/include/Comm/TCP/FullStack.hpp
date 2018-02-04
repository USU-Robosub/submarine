#ifndef COMM_TCP_FULL_STACK
#define COMM_TCP_FULL_STACK

#include <string>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <Comm/Hub.hpp>

namespace Comm{
  namespace TCP{
    class FullStack;
  }
}

class Comm::TCP::FullStack{
public:
  FullStack & operator=(const FullStack&) = delete;
  FullStack(const FullStack&) = delete;
  FullStack(std::string address, unsigned int portNum, char separator);
  FullStack(unsigned int portNum, char separator);
  FullStack(Comm::TCP::Port port, char separator);
  ~FullStack();
  Comm::Hub<std::string>* hub();
private:
  Comm::TCP::SingleClientServer* server;
  Comm::TCP::Port port;
  Comm::TCP::Stream stream;
  Comm::TCP::Bridge bridge;
  Comm::Hub<std::string> _hub;
};

#endif
