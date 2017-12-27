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
  virtual void emit(T name, std::vector<T> message);
  virtual Comm::HubBinding<T> on(T name, Comm::HandlerFunction<T> handler);
  virtual void poll();
  virtual void remove(Comm::HubBinding<T> binding);

protected:
  Bridge<T>* bridge;
  std::map<T, std::map<unsigned int, Comm::HandlerFunction<T>>> handlers;
  std::map<T, unsigned int> nextIDs;
};

template<class T>
struct Comm::HubBinding{
  T name;
  unsigned int id;
};

#include "../../src/Comm/Hub.ipp"

#endif
