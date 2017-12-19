#ifndef COMM_HUB
#define COMM_HUB

#include <vector>
#include <functional>
#include <map>
#include <queue>
#include <Comm/Bridge.h>

namespace Comm{
  template<class T>
  using handlerFunction = std::function<void(std::vector<T>)>;

  template<class T>
  class Hub;
}

template<class T>
class Comm::Hub{
public:
  Hub() {}
  Hub(Comm::Bridge<T>* bridge);
  void emit(T name, std::vector<T> message);
  void on(T name, handlerFunction<T> handler);
  void poll();

private:
  Bridge<T>* bridge;
  std::map<T, std::vector<handlerFunction<T>>> handlers;
};

#endif
