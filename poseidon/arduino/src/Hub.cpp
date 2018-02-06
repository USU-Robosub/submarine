#include <Hub.hpp>

Hub::Hub(Controller** controllers, int numControllers)
: _controllers(controllers)
, _numControllers(numControllers)
, state(MessageState::CHECK)
, currentMessage()
, dataLeft(0)
{
  Serial.begin(115200);//115200
  while(!Serial){}
}

void Hub::serveEvent(){
  if(this->currentMessage.length < this->_numControllers){
    _controllers[this->currentMessage.name]->execute(this, this->currentMessage.data, this->currentMessage.length);
  }
}

// fast fall through, the arduino serial buffer(s) are very small
void Hub::poll()
{


  this->emit(10, 0, 0);
  if(Serial.available() > 4){
    this->emit(10, 0, 0);
    switch(this->state){
      case MessageState::CHECK:
        this->currentMessage.check = this->readOneLong();
        if(this->currentMessage.check == 0){
          this->state = MessageState::NAME;
        }
        break;
      case MessageState::NAME:
        this->currentMessage.name = this->readOneLong();
        this->state = MessageState::LENGTH;
        break;
      case MessageState::LENGTH:
        this->currentMessage.length = this->readOneLong();
        this->dataLeft = this->currentMessage.length;
        if(this->currentMessage.data != nullptr){
          delete this->currentMessage.data;
          this->currentMessage.data = nullptr;
        }
        if(this->dataLeft > 1000){
          this->state = MessageState::CHECK;
        }else if(this->dataLeft > 0){
          this->currentMessage.data = new long[this->currentMessage.length];
          this->state = MessageState::DATA;
        }else{
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
      case MessageState::DATA:
        long dataIndex = this->currentMessage.length - this->dataLeft;
        this->currentMessage.data[dataIndex] = this->readOneLong();
        --this->dataLeft;
        if(this->dataLeft == 0){
          this->serveEvent();
          this->state = MessageState::CHECK;
        }
        break;
    }
  }


  // // =====================
  // if(Serial.available() > 0){
  //   long null = readOneLong();
  //   if(null != 0){
  //     _controllers[0]->execute(this, 0, 0);
  //     return;
  //   }
  //   long name = readOneLong();
  //   long length = readOneLong();
  //   long* data = new long[length];
  //   for(long i = 0; i < length; ++i){
  //     data[i] = readOneLong();
  //   }
  //   if(name >= _numControllers)
  //   {
  //     _controllers[0]->execute(this, 0, 0);
  //     emit(0, &name, 1);
  //   } else
  //   {
  //     _controllers[name]->execute(this, data, length);
  //   }
  //   delete data;
  // }
}

void Hub::emit(long name, long* data, long length)
{
  long null = 0;
  writeOneLong(null);
  writeOneLong(name);
  writeOneLong(length);
  for(long i = 0; i < length; ++i){
    writeOneLong(data[i]);
  }
}

long Hub::readOneLong()
{
  return (this->read()) |
         (this->read() << 8) |
         (this->read() << 16) |
         (this->read() << 24);
}

void Hub::writeOneLong(long value)
{
  Serial.write((char*)&value, 4);
}

long Hub::read(){
  return Serial.read();
}
