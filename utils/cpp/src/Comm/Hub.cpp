#include <Comm/Hub.hpp>

template <class T>
Comm::Hub<T>::Hub(Bridge<T>* bridge) : bridge(bridge) {

}

template <class T>
void Comm::Hub<T>::emit(T name, std::vector<T> message){
  message.push_back(name);
  bridge->send(message);
}
