#ifndef COMM_SERIAL_INT_STREAM_WRAPPER
#define COMM_SERIAL_INT_STREAM_WRAPPER

#include <cstring>
#include <mutex>
#include "Comm/Stream.h"
#include "Comm/Hub.hpp"

#define COMM_INT_SIZE 4

namespace Comm{
  class IntStreamWrapper{
  public:
    IntStreamWrapper(Stream* stream) : stream(stream) {}
    void lock();
    void unlock();
    virtual int readInt();
    virtual void writeInt(int value);
    virtual bool hasData();
  private:
    Stream* stream;
    std::mutex threadLock;
    bool isLocked = false;
  };
}

#endif
