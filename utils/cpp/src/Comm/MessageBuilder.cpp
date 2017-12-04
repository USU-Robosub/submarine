#include "Comm/MessageBuilder.h"

#include <iostream>

Comm::Message Comm::MessageBuilder::read(){
  char* rawData = new char[this->bufferLength];
  this->stream->receive(rawData, this->bufferLength);

  unsigned char length = rawData[0];

  char* data = new char[length];
  for(unsigned int i = 0; i < length; ++i){
    data[i] = rawData[i + 1];
  }

  return Comm::Message(data, length);
}
