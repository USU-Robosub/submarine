#ifndef COMM_HUB
#define COMM_HUB

#include <Comm/Bridge.h>
#include <vector>

namespace Comm{
  template<class T>
  class Hub{
  public:
    Hub() {}
    Hub(Bridge<T>* bridge);
    void emit(T name, std::vector<T> message);
  private:
    Bridge<T>* bridge;
  };
}

#endif
