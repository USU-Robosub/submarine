#ifndef COMM_PORT
#define COMM_PORT

#include <cstddef>
#include <stdexcept>

namespace Comm{
  template<class T>
  class Port;
  class ConnectionFailure;
}

template<class T>
class Comm::Port{
public:
  Port & operator=(const Port&) = delete;
  Port(const Port&) = delete;
  Port() = default;
  virtual void push(const T* buffer, std::size_t length) = 0;
  virtual std::size_t poll(T* buffer, std::size_t length) = 0;
  virtual bool hasData() = 0;
};

class Comm::ConnectionFailure : public std::runtime_error{
public:
  ConnectionFailure(const char* what) : std::runtime_error(what) {};
};

#endif
