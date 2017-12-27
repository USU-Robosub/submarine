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
Comm::HubBinding<T> Comm::Hub<T>::on(T name, HandlerFunction<T> handler){
  unsigned int id = this->nextIDs[name]++;
  this->handlers[name][id] = handler;
  return Comm::HubBinding<T>{.name=name, .id=id};
}

template <class T>
void Comm::Hub<T>::poll(){
  std::queue<std::vector<T>> messages = this->bridge->receive();
  while(!messages.empty()){
    std::vector<T> message = messages.front();
    auto handlers = this->handlers[message.back()];
    message.erase(message.end() - 1);
    for(const auto& handler : handlers){
      handler.second(message);
    }
    messages.pop();
  }
}

template <class T>
void Comm::Hub<T>::remove(Comm::HubBinding<T> binding){
  this->handlers[binding.name].erase(binding.id);
}
