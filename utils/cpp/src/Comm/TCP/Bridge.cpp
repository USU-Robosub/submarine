#include <Comm/TCP/Bridge.hpp>

Comm::TCP::Bridge::Bridge(Comm::Stream<std::string>* stream)
  : stream(stream){ }

void Comm::TCP::Bridge::send(std::vector<std::string> data){
  this->stream->push("0");
  this->stream->push(data.back());
  this->stream->push(std::to_string(data.size() - 1));
  for(unsigned int i = 0; i < data.size() - 1; ++i)
    this->stream->push(data[i]);
}

std::queue<std::vector<std::string>> Comm::TCP::Bridge::receive(){
  std::queue<std::vector<std::string>> messages;
  bool errored = false;
  while(!errored && this->stream->hasData()){
    try{
      this->stream->poll(); // burn the check number
      std::vector<std::string> message;
      std::string name = this->stream->poll();
      int length = std::stoi(this->stream->poll());
      for(unsigned int i = 0; i < length; ++i){
        message.push_back(this->stream->poll());
      }
      message.push_back(name);
      messages.push(message);
    }catch(...){
      errored = true;
    }
  }
  return messages;
}
