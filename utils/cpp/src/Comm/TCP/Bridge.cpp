#include <Comm/TCP/Bridge.hpp>

Comm::TCP::Bridge::Bridge(Comm::Stream<std::string>* stream)
  : stream(stream),
    state(Comm::MessageState::CHECK),
    currentMessage(),
    dataLeft(0){ }

void Comm::TCP::Bridge::send(std::vector<std::string> data){
  this->stream->push("0");
  this->stream->push(data.back());
  this->stream->push(std::to_string(data.size() - 1));
  for(unsigned int i = 0; i < data.size() - 1; ++i)
    this->stream->push(data[i]);
}

std::queue<std::vector<std::string>> Comm::TCP::Bridge::receive(){
  std::queue<std::vector<std::string>> messages;
  while(this->stream->hasData()){
    switch(this->state){
      case Comm::MessageState::CHECK:
        this->currentMessage.check = this->stream->poll();
        this->state = Comm::MessageState::NAME;
        break;
      case Comm::MessageState::NAME:
        this->currentMessage.name = this->stream->poll();
        this->state = Comm::MessageState::LENGTH;
        break;
      case Comm::MessageState::LENGTH:
        this->currentMessage.length = this->stream->poll();
        this->dataLeft = std::stoi(this->currentMessage.length);
        if(this->dataLeft > 0){
          this->state = Comm::MessageState::DATA;
        }else{
          messages.push(std::vector<std::string>{this->currentMessage.name});
          this->state = Comm::MessageState::CHECK;
        }
        break;
      case Comm::MessageState::DATA:
        this->currentMessage.data.push_back(this->stream->poll());
        --this->dataLeft;
        if(this->dataLeft == 0){
          this->currentMessage.data.push_back(this->currentMessage.name);
          messages.push(this->currentMessage.data);
          this->currentMessage = Comm::Message<std::string>();
          this->state = Comm::MessageState::CHECK;
        }
        break;
    }
  }
  return messages;
}
