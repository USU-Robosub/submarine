#ifndef COMM_STREAM
#define COMM_STREAM

namespace Comm{
  template<class T>
  class Stream;
}

template<class T>
class Comm::Stream{
public:
  virtual bool hasData() = 0;
  virtual void push(T data) = 0;
  virtual T poll() = 0;
};

#endif
