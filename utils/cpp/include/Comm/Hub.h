#ifndef COMM_HUB
#define COMM_HUB

namespace Comm{
  class Hub{
  public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual int readInt() = 0;
    virtual void writeInt(int value) = 0;
  };
}

#endif
