#ifndef COMM_SERIAL_STREAM
#define COMM_SERIAL_STREAM

#include <cstring>
#include <Comm/Stream.hpp>
#include <Comm/Port.hpp>

#define COMM_INT_SIZE 4

namespace Comm{
  namespace Serial{
    class Stream;
  }
}

class Comm::Serial::Stream : public Comm::Stream<int>{
public:
  Stream(Comm::Port<unsigned char>* port);
  void lock();
  void unlock();
  int poll();
  void push(int value);
  bool hasData();

private:
  Comm::Port<unsigned char>* port;
  bool isLocked;
};

#endif
