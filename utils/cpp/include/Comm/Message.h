#ifndef COMM_MESSAGE
#define COMM_MESSAGE

namespace Comm{
  class Message{
  public:
    Message(char* data, unsigned int length) : data(data), length(length) {}
    char* data;
    unsigned int length;
  };
}

#endif
