#ifndef COMM_BINARY_HUB
#define COMM_BINARY_HUB

#include <cstring>
#include <mutex>
#include "Comm/Stream.h"
#include "Comm/Hub.h"

#define COMM_INT_SIZE 4

namespace Comm{
  class BinaryHub : public Hub{
  public:
    BinaryHub(Stream* stream) : stream(stream) {}
    void lock();
    void unlock();
    int readInt();
    void writeInt(int value);
  private:
    Stream* stream;
    std::mutex threadLock;
    bool isLocked = false;
  };
}

#endif
