#ifndef COMM_BRIDGE
#define COMM_BRIDGE

#include <vector>
#include <queue>

namespace Comm{
  template<class T>
  class Bridge{
  public:
    virtual void send(std::vector<T> data) = 0;
    virtual std::queue<std::vector<T>> receive() = 0;
  };
}

#endif
