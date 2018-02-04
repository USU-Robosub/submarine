#include <Comm/Serial/Bridge.hpp>

Comm::Serial::Bridge::Bridge(Comm::Stream<int>* stream, unsigned int maxReads)
  : stream(stream),
    state(Comm::MessageState::CHECK),
    currentMessage(),
    dataLeft(0),
    maxReads(maxReads){ }

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
  unsigned int count = 0;
  while(this->stream->hasData() && count < this->maxReads){
    ++count;
    switch(this->state){
      case Comm::MessageState::CHECK:
        this->currentMessage.check = this->stream->poll();
        if(this->currentMessage.check == 0){
          this->state = Comm::MessageState::NAME;
        }
        break;
      case Comm::MessageState::NAME:
        this->currentMessage.name = this->stream->poll();
        this->state = Comm::MessageState::LENGTH;
        break;
      case Comm::MessageState::LENGTH:
        this->currentMessage.length = this->stream->poll();
        this->dataLeft = this->currentMessage.length;
        if(this->dataLeft > 1000){
          throw new Comm::ConnectionFailure(("Serial message is very large, [ints] " + std::to_string(this->dataLeft)).c_str());
        }else if(this->dataLeft > 0){
          this->state = Comm::MessageState::DATA;
        }else{
          messages.push(std::vector<int>{this->currentMessage.name});
          this->state = Comm::MessageState::CHECK;
        }
        break;
      case Comm::MessageState::DATA:
        this->currentMessage.data.push_back(this->stream->poll());
        --this->dataLeft;
        if(this->dataLeft == 0){
          this->currentMessage.data.push_back(this->currentMessage.name);
          messages.push(this->currentMessage.data);
          this->currentMessage = Comm::Message<int>();
          this->state = Comm::MessageState::CHECK;
        }
        break;
    }
  }
  return messages;
}
