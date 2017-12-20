#include <Comm/Hub.hpp>

template <class T>
Comm::Hub<T>::Hub(Bridge<T>* bridge)
 : bridge(bridge) { }

template <class T>
void Comm::Hub<T>::emit(T name, std::vector<T> message){
  message.push_back(name);
  this->bridge->send(message);
}

template <class T>
void Comm::Hub<T>::on(T name, handlerFunction<T> handler){
  this->handlers[name].push_back(handler);
}

template <class T>
void Comm::Hub<T>::poll(){
  std::queue<std::vector<T>> messages = this->bridge->receive();
  while(!messages.empty()){
    std::vector<T> message = messages.front();
    std::vector<handlerFunction<T>> handlers = this->handlers[message.back()];
    message.erase(message.end() - 1);
    for(unsigned int i = 0; i < handlers.size(); ++i){
      handlers[i](message);
    }
    messages.pop();
  }
}
