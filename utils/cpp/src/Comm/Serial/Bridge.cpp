#include <Comm/Serial/Bridge.hpp>

Comm::Serial::Bridge::Bridge(Comm::Stream<int>* stream)
  : stream(stream) { }

void Comm::Serial::Bridge::send(std::vector<int> data){
  this->stream->push(0);
  this->stream->push(data.back());
  this->stream->push(data.size() - 1);
  for(unsigned int i = 0; i < data.size() - 1; ++i){
    this->stream->push(data[i]);
  }
}

std::queue<std::vector<int>> Comm::Serial::Bridge::receive(){
  std::queue<std::vector<int>> messages;
  while(this->stream->hasData()){
    this->stream->poll(); // burn the check number
    std::vector<int> message;
    int name = this->stream->poll();
    int length = this->stream->poll();
    for(unsigned int i = 0; i < length; ++i){
      message.push_back(this->stream->poll());
    }
    message.push_back(name);
    messages.push(message);
  }
  return messages;
}
