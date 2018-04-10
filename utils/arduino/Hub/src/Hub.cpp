#include <Hub.hpp>

Hub::Hub(Controller** controllers, int numControllers, int maxReadsPerPoll)
: _controllers(controllers)
, _numControllers(numControllers)
, state(MessageState::CHECK)
, currentMessage()
, dataLeft(0)
, maxReadsPerPoll(maxReadsPerPoll)
{
  Serial.begin(115200);//115200
  while(!Serial){}
}

void Hub::serveEvent(){
  if(this->currentMessage.name < this->_numControllers){
    _controllers[this->currentMessage.name]->execute(this, this->currentMessage.data, this->currentMessage.length);
  }
}

void Hub::poll()
{
  int count = 0;
  while(Serial.available() >= 4 && count < this->maxReadsPerPoll){
    ++count;
    switch(this->state){
      case MessageState::CHECK:
        this->currentMessage.check = this->readInt();
        if(this->currentMessage.check == 0){
          this->state = MessageState::NAME;
        }
        break;
      case MessageState::NAME:
        this->currentMessage.name = this->readInt();
        this->state = MessageState::LENGTH;
        break;
      case MessageState::LENGTH:
        this->currentMessage.length = this->readInt();
        this->dataLeft = this->currentMessage.length;
        if(this->currentMessage.data != nullptr){
          delete this->currentMessage.data;
          this->currentMessage.data = nullptr;
        }
        if(this->dataLeft > 1000){
          this->state = MessageState::CHECK;
        }else if(this->dataLeft > 0){
          this->currentMessage.data = new int32_t[this->currentMessage.length];
          this->state = MessageState::DATA;
        }else{
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
      case MessageState::DATA:{
        int32_t dataIndex = this->currentMessage.length - this->dataLeft;
        this->currentMessage.data[dataIndex] = this->readInt();
        --this->dataLeft;
        if(this->dataLeft == 0){
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
      }
      case MessageState::ALIGNING:
        //Not sure what should be done here but it is complaining that it is not implemented
        break;
    }
  }
}

void Hub::emit(int32_t name, int32_t* data, int32_t length)
{
  int32_t null = 0;
  writeInt(null);
  writeInt(name);
  writeInt(length);
  for(int32_t i = 0; i < length; ++i){
    writeInt(data[i]);
  }
}

int32_t Hub::readInt()
{
  return (this->read()) |
         (this->read() << 8) |
         (this->read() << 16) |
         (this->read() << 24);
}

void Hub::writeInt(int32_t value)
{
  Serial.write((unsigned char*)&value, sizeof(int32_t));
}

int32_t Hub::read(){
  return Serial.read();
}

void Hub::kill(){
  for(int32_t i = 0; i < _numControllers; i++)  {
    _controllers->kill();
  }
}

void Hub::restart()
{
  for(int32_t i = 0; i < _numControllers; i++)  {
    _controllers->restart();
  }
}