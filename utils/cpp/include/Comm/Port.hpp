#ifndef COMM_PORT
#define COMM_PORT

#include <Threading/Lockable.hpp>

namespace Comm{
  template<class T>
  class Port;
}

template<class T>
class Comm::Port : public Threading::Lockable{
public:
  virtual void push(const T* buffer, unsigned int length) = 0;
  virtual void poll(T* buffer, unsigned int length) = 0;
  virtual bool hasData() = 0;
};

#endif
