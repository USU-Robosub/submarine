#ifndef COMM_BINARY_PORT_MOCK
#define COMM_BINARY_PORT_MOCK

#include <Comm/BinaryPort.hpp>

namespace Comm{
  namespace Mock{
    class BinaryPort;
  }
}

class Comm::Mock::BinaryPort : public Comm::BinaryPort{
public:
  BinaryPort();
  void poll(unsigned char* buffer, unsigned int length);
  void push(unsigned char* buffer, unsigned int length);
  bool hasData();
  void lock();
  void unlock();

  unsigned char* buffer;
  bool dataAvailable;
  bool locked;
  bool unlocked;

private:
  unsigned int bufferIndex;
};

Comm::Mock::BinaryPort::BinaryPort()
  : bufferIndex(0),
    dataAvailable(true),
    locked(false),
    unlocked(false) {}

void Comm::Mock::BinaryPort::poll(unsigned char* buffer, unsigned int length) {
  for(unsigned int i = 0; i < length; ++i)
    buffer[i] = this->buffer[this->bufferIndex++];
}

void Comm::Mock::BinaryPort::push(unsigned char* buffer, unsigned int length){
  for(unsigned int i = 0; i < length; ++i)
    this->buffer[this->bufferIndex++] = buffer[i];
}

bool Comm::Mock::BinaryPort::hasData(){
  return this->dataAvailable;
}

void Comm::Mock::BinaryPort::lock(){
  this->locked = true;
}

void Comm::Mock::BinaryPort::unlock(){
  this->unlocked = true;
}

#endif
