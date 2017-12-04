#ifndef COMM_BINARY_HUB
#define COMM_BINARY_HUB

#include <cstring>
#include <mutex>
#include "Comm/Stream.h"

#define COMM_INT_SIZE 4

namespace Comm{
  class BinaryHub{
  public:
    BinaryHub(Stream* stream) : stream(stream) {}
    int readInt();
    void writeInt(int value);
    void lock();
    void unlock();
  private:
    Stream* stream;
    std::mutex threadLock;
    bool isLocked = false;
  };
}

#endif
