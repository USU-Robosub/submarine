#ifndef COMM_SERIAL_FULL_STACK
#define COMM_SERIAL_FULL_STACK

#include <string>
#include <Comm/Serial/Port.hpp>
#include <Comm/Serial/Stream.hpp>
#include <Comm/Serial/Bridge.hpp>
#include <Comm/Hub.hpp>
#include <iostream>

namespace Comm{
  namespace Serial{
    class FullStack;
  }
}

class Comm::Serial::FullStack{
public:
  FullStack & operator=(const FullStack&) = delete;
  FullStack(const FullStack&) = delete;
  FullStack(std::string portName, speed_t speed);
  Comm::Hub<int>* hub();
private:
  Comm::Serial::Port port;
  Comm::Serial::Stream stream;
  Comm::Serial::Bridge bridge;
  Comm::Hub<int> _hub;
};

#endif
