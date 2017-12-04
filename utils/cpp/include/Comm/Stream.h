#ifndef COMM_STREAM
#define COMM_STREAM

#include "Comm/Status.h"

namespace Comm{
  class Stream{
  public:
    virtual Status status() = 0;
    virtual bool canRead() = 0;
    virtual void send(char* data, unsigned int length) = 0;
    virtual void receive(char* data, unsigned int length) = 0;
  };
}

#endif
