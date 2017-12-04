#include "Comm/BinaryHub.h"

/*

Notice: This code is effected by the system's memory layout (little endian or big endian).

*/

int Comm::BinaryHub::readInt(){
  if(this->isLocked == false)
    return 0;
  int result;
  char binary[COMM_INT_SIZE];
  this->stream->receive(binary, COMM_INT_SIZE);
  memcpy(&result, binary, COMM_INT_SIZE);
  return result;
}

void Comm::BinaryHub::writeInt(int value){
  if(this->isLocked == false)
    return;
  char binary[COMM_INT_SIZE];
  memcpy(binary, &value, COMM_INT_SIZE);
  this->stream->send(binary, COMM_INT_SIZE);
}

void Comm::BinaryHub::lock(){
  this->threadLock.lock();
  this->isLocked = true;
}

void Comm::BinaryHub::unlock(){
  this->threadLock.unlock();
  this->isLocked = false;
}
