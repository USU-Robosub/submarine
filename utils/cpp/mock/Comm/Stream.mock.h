#ifndef COMM_MESSAGE_BUILDER_MOCK
#define COMM_MESSAGE_BUILDER_MOCK

#include "Comm/Stream.h"

namespace Comm{
  namespace Mock{
    class TestStream : public Stream{
    public:
      Status status() {}
      bool canRead() {}
      
      void send(char* data, unsigned int length) {
        for(unsigned int i = 0; i < length; ++i){
          this->data[this->writeIndex++] = data[i];
        }
      }

      void receive(char* data, unsigned int length) {
        for(unsigned int i = 0; i < length; ++i){
          data[i] = this->data[this->readIndex++];
        }
      }

      char* data = nullptr;
    private:
      unsigned int readIndex = 0;
      unsigned int writeIndex = 0;
    };
  }
}

#endif
