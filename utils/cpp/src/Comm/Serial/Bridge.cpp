#include <Comm/Serial/Bridge.hpp>

Comm::Serial::Bridge::Bridge(Comm::IntStreamWrapper* stream)
  : stream(stream) { }

void Comm::Serial::Bridge::send(std::vector<int> data){
  this->stream->writeInt(0);
  this->stream->writeInt(data.back());
  this->stream->writeInt(data.size() - 1);
  for(unsigned int i = 0; i < data.size() - 1; ++i){
    this->stream->writeInt(data[i]);
  }
}

std::queue<std::vector<int>> Comm::Serial::Bridge::receive(){
  std::queue<std::vector<int>> messages;
  while(this->stream->hasData()){
    this->stream->readInt(); // burn the check number
    std::vector<int> message;
    int name = this->stream->readInt();
    int length = this->stream->readInt();
    for(unsigned int i = 0; i < length; ++i){
      message.push_back(this->stream->readInt());
    }
    message.push_back(name);
    messages.push(message);
  }
  return messages;
}
