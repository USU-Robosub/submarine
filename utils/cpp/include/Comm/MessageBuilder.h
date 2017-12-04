#ifndef COMM_MESSAGE_BUILDER
#define COMM_MESSAGE_BUILDER

#include "Comm/Message.h"
#include "Comm/Stream.h"

namespace Comm{
  class MessageBuilder{
  public:
    MessageBuilder(Stream* stream) : stream(stream) {}
    Message read();
  private:
    unsigned int bufferLength = 6;
    Stream* stream;
  };
}

#endif
