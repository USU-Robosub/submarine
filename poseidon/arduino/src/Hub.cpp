#include "Hub.h"

Hub::Hub(Controller* controllers, int numControllers)
: _controllers(controllers)
, _numControllers(numControllers)
{
  Serial.begin(9600);
  while(!Serial){}
}

void poll()
{
  long null = readOneLong();
  if(null != 0){
    controllers[0]->execute(hub, 0, 0);
    std::cerr << "Serial message misaligned: '" << null << "'" << std::endl;
    return;
  }
  long command = readOneLong();
  long length = readOneLong();
  long* data = new long[length];
  for(unsigned long i = 0; i < length; ++i){
    data[i] = readOneLong();
  }
  if(command >= numControllers)
  {
    controllers[0]->execute(hub, 0, 0);
    emit(0, &name, 1);
  } else
  {
    controllers[command]->execute(this, data, length);
  }
  delete data;
}

void emit(long name, long* data, long length)
{
  long null = 0;
  writeOneLong(null);
  writeOneLong(name);
  writeOneLong(length);
  for(long i = 0; i < length; ++i){
    writeOneLong(data[i]);
  }
}

long readOneLong()
{
  long n = Serial.read();
  n += Serial.read() << 1;
  n += Serial.read() << 2;
  n += Serial.read() << 3;
  return n;
}

void writeOneLong(long value)
{
  Serial.write((char*)&value, 4);
}
