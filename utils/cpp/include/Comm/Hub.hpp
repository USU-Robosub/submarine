#ifndef COMM_HUB
#define COMM_HUB

#include <vector>
#include <functional>
#include <map>
#include <queue>
#include <Comm/Bridge.h>

namespace Comm{
  template<class T>
  struct HubBinding;

  template<class T>
  using HandlerFunction = std::function<void(std::vector<T>)>;

  template<class T>
  class Hub;
}

template<class T>
class Comm::Hub{
public:
  Hub(Comm::Bridge<T>* bridge);
  void emit(T name, std::vector<T> message);
  Comm::HubBinding<T> on(T name, HandlerFunction<T> handler);
  void poll();
  void remove(Comm::HubBinding<T> binding);

private:
  Bridge<T>* bridge;
  std::map<T, std::map<unsigned int, HandlerFunction<T>>> handlers;
  std::map<T, unsigned int> nextIDs;
};

template<class T>
struct Comm::HubBinding{
  T name;
  unsigned int id;
};

#include "../../src/Comm/Hub.tpp"

#endif
