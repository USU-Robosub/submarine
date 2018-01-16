#ifndef COMM_PORT_MOCK
#define COMM_PORT_MOCK

#include <Comm/Port.hpp>

namespace Comm{
  namespace Mock{
    template<class T>
    class Port;
  }
}

template<class T>
class Comm::Mock::Port : public Comm::Port<T>{
public:
  Port();
  std::size_t poll(T* buffer, std::size_t length);
  void push(const T* buffer, std::size_t length);
  bool hasData();
  void lock();
  void unlock();

  T* buffer;
  bool isLocked;
  unsigned int bufferIndex;
  unsigned int bufferLength;
};

template<class T>
Comm::Mock::Port<T>::Port()
  : buffer(nullptr),
    isLocked(false),
    bufferIndex(0),
    bufferLength(0) {}

template<class T>
std::size_t Comm::Mock::Port<T>::poll(T* buffer, std::size_t length) {
  unsigned int i = 0;
  for(; i < length && this->bufferIndex < this->bufferLength; ++i)
    buffer[i] = this->buffer[this->bufferIndex++];
  return i;
}

template<class T>
void Comm::Mock::Port<T>::push(const T* buffer, std::size_t length){
  for(unsigned int i = 0; i < length && this->bufferIndex < this->bufferLength; ++i)
    this->buffer[this->bufferIndex++] = buffer[i];
}

template<class T>
bool Comm::Mock::Port<T>::hasData(){
  return this->bufferIndex < this->bufferLength;
}

template<class T>
void Comm::Mock::Port<T>::lock(){
  this->isLocked = true;
}

template<class T>
void Comm::Mock::Port<T>::unlock(){
  this->isLocked = false;
}

#endif
