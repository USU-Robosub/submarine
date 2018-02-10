#include <Comm/Serial/Stream.hpp>

/*

Notice: This code is effected by the system's memory layout (little endian or big endian).

*/

Comm::Serial::Stream::Stream(Comm::Port<unsigned char>* port)
  : port(port) { }

int Comm::Serial::Stream::poll(){
  int result;
  unsigned char buffer[COMM_INT_SIZE];
  this->port->poll(buffer, COMM_INT_SIZE);
  memcpy(&result, buffer, COMM_INT_SIZE);
  return result;
}

void Comm::Serial::Stream::push(int value){
  unsigned char binary[COMM_INT_SIZE];
  memcpy(binary, &value, COMM_INT_SIZE);
  this->port->push(binary, COMM_INT_SIZE);
}

bool Comm::Serial::Stream::hasData(){
  return this->port->hasData();
}
