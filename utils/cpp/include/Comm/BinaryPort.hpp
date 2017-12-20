#ifndef COMM_SERIAL_BINARY_PORT
#define COMM_SERIAL_BINARY_PORT

namespace Comm{
  class BinaryPort;
}

class Comm::BinaryPort{
public:
  virtual void push(unsigned char* buffer, unsigned int length) = 0;
  virtual void poll(unsigned char* buffer, unsigned int length) = 0;
  virtual void lock() = 0;
  virtual void unlock() = 0;
};

#endif
