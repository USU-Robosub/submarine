#include <Comm/Serial/IntStreamWrapper.hpp>

/*

Notice: This code is effected by the system's memory layout (little endian or big endian).

*/

Comm::Serial::IntStreamWrapper::IntStreamWrapper(Comm::BinaryPort* port)
  : port(port),
    isLocked(false) {
    this->lock();
  }

int Comm::Serial::IntStreamWrapper::poll(){
  if(this->isLocked == false)
    return 0;
  int result;
  unsigned char buffer[COMM_INT_SIZE];
  this->port->poll(buffer, COMM_INT_SIZE);
  memcpy(&result, buffer, COMM_INT_SIZE);
  return result;
}

void Comm::Serial::IntStreamWrapper::push(int value){
  if(this->isLocked == false)
    return;
  unsigned char binary[COMM_INT_SIZE];
  memcpy(binary, &value, COMM_INT_SIZE);
  this->port->push(binary, COMM_INT_SIZE);
}

void Comm::Serial::IntStreamWrapper::lock(){
  this->port->lock();
  this->isLocked = true;
}

void Comm::Serial::IntStreamWrapper::unlock(){
  this->isLocked = false;
  this->port->unlock();
}

bool Comm::Serial::IntStreamWrapper::hasData(){
  //return true;
}
