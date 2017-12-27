#ifndef COMM_SERIAL_STREAM
#define COMM_SERIAL_STREAM

#include <cstring>
#include <Comm/Stream.hpp>
#include <Comm/BinaryPort.hpp>

#define COMM_INT_SIZE 4

namespace Comm{
  namespace Serial{
    class Stream;
  }
}

class Comm::Serial::Stream : public Comm::Stream<int>{
public:
  Stream(Comm::BinaryPort* port);
  void lock();
  void unlock();
  int poll();
  void push(int value);
  bool hasData();
  
private:
  Comm::BinaryPort* port;
  bool isLocked;
};

#endif
