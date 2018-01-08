#ifndef COMM_PORT
#define COMM_PORT

#include <cstddef>
#include <Threading/Lockable.hpp>

namespace Comm{
  template<class T>
  class Port;
}

template<class T>
class Comm::Port : public Threading::Lockable{
public:
  Port & operator=(const Port&) = delete;
  Port(const Port&) = delete;
  Port() = default;
  virtual void push(const T* buffer, std::size_t length) = 0;
  virtual std::size_t poll(T* buffer, std::size_t length) = 0;
  virtual bool hasData() = 0;
};

#endif
