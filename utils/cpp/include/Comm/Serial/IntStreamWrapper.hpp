#ifndef COMM_SERIAL_INT_STREAM_WRAPPER
#define COMM_SERIAL_INT_STREAM_WRAPPER

#include <cstring>
#include <mutex>
#include <Comm/Stream.hpp>
#include <Comm/Serial/Serial.hpp>
#include <Comm/Hub.hpp>

#define COMM_INT_SIZE 4

namespace Comm{
  namespace Serial{
    class IntStreamWrapper;
  }
}

class Comm::Serial::IntStreamWrapper : public Comm::Stream<int>{
public:
  IntStreamWrapper(Comm::BinaryPort* port);
  void lock();
  void unlock();
  int poll();
  void push(int value);
  bool hasData();
private:
  Comm::BinaryPort* port;
  std::mutex threadLock;
  bool isLocked;
};

#endif
