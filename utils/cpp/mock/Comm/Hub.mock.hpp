#ifndef COMM_HUB_MOCK
#define COMM_HUB_MOCK

#include <vector>
#include <Comm/Hub.hpp>
#include <Comm/Bridge.mock.hpp>

namespace Comm{
  namespace Mock{
    template<class T>
    class Hub;
  }
}

template<class T>
class Comm::Mock::Hub : public Comm::Hub<T>{
public:
  Hub();
  void emit(T name, std::vector<T> message);
  Comm::HubBinding<T> on(T name, Comm::HandlerFunction<T> handler);
  void poll();
  void remove(Comm::HubBinding<T> binding);
  ~Hub();

  std::vector<std::pair<T, Comm::HandlerFunction<T>>> handlers;
  std::vector<std::pair<T, std::vector<T>>> messages;
};

template<class T>
Comm::Mock::Hub<T>::Hub() : Comm::Hub<T>(new Comm::Mock::Bridge<T>) { }

template<class T>
Comm::Mock::Hub<T>::~Hub() {
  delete this->bridge;
}

template<class T>
void Comm::Mock::Hub<T>::emit(T name, std::vector<T> message){
  this->messages.push_back({name, message});
}

template<class T>
Comm::HubBinding<T> Comm::Mock::Hub<T>::on(T name, HandlerFunction<T> handler){
  this->handlers.push_back({name, handler});
  return Comm::HubBinding<T>{};
}

template<class T>
void Comm::Mock::Hub<T>::poll(){

}

template<class T>
void Comm::Mock::Hub<T>::remove(Comm::HubBinding<T> binding){

}

#endif
