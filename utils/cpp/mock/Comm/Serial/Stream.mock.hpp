#ifndef COMM_SERIAL_STREAM_MOCK
#define COMM_SERIAL_STREAM_MOCK

#include "Comm/Stream.h"

namespace Comm{
  namespace Serial{
    namespace Mock{
      class Stream;
    }
  }
}

class Comm::Serial::Mock::Stream{
public:
  bool isReadable() { }

  void push(char* data, unsigned int length) {
    for(unsigned int i = 0; i < length; ++i){
      this->data[this->writeIndex++] = data[i];
    }
  }

  void poll(char* data, unsigned int length) {
    for(unsigned int i = 0; i < length; ++i){
      data[i] = this->data[this->readIndex++];
    }
  }

  char* data = nullptr;
private:
  unsigned int readIndex = 0;
  unsigned int writeIndex = 0;
};

#endif
