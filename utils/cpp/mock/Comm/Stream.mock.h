#ifndef COMM_MESSAGE_BUILDER_MOCK
#define COMM_MESSAGE_BUILDER_MOCK

#include "Comm/Stream.h"

namespace Comm{
  namespace Mock{
    class TestStream : public Stream{
    public:
      Status status() {};
      bool canRead() {};
      void send(char* data, unsigned int length) {};
      void receive(char* data, unsigned int length) {
        for(unsigned int i = 0; i < length; ++i){
          data[i] = this->data[i];
        }
      };
      char* data;
    };
  }
}

#endif
