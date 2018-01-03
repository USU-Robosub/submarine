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
  void poll(T* buffer, unsigned int length);
  void push(const T* buffer, unsigned int length);
  bool hasData();
  void lock();
  void unlock();

  T* buffer;
  bool dataAvailable;
  bool isLocked;
  unsigned int bufferIndex;
};

template<class T>
Comm::Mock::Port<T>::Port()
  : buffer(nullptr),
    dataAvailable(true),
    isLocked(false),
    bufferIndex(0) {}

template<class T>
void Comm::Mock::Port<T>::poll(T* buffer, unsigned int length) {
  for(unsigned int i = 0; i < length; ++i)
    buffer[i] = this->buffer[this->bufferIndex++];
}

template<class T>
void Comm::Mock::Port<T>::push(const T* buffer, unsigned int length){
  for(unsigned int i = 0; i < length; ++i)
    this->buffer[this->bufferIndex++] = buffer[i];
}

template<class T>
bool Comm::Mock::Port<T>::hasData(){
  return this->dataAvailable;
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
