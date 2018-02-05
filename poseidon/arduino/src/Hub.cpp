#include <Hub.hpp>

Hub::Hub(Controller** controllers, int numControllers)
: _controllers(controllers)
, _numControllers(numControllers)
{
  Serial.begin(115200);//115200
  while(!Serial){}
}

void Hub::poll()
{
  if(Serial.available() >= 4*4){
    long null = readOneLong();
    if(null != 0){
      _controllers[0]->execute(this, 0, 0);
      return;
    }
    long name = readOneLong();
    long length = readOneLong();
    long* data = new long[length];
    for(long i = 0; i < length; ++i){
      data[i] = readOneLong();
    }
    if(name >= _numControllers)
    {
      _controllers[0]->execute(this, 0, 0);
      emit(0, &name, 1);
    } else
    {
      _controllers[name]->execute(this, data, length);
    }
    delete data;
  }
}

void Hub::emit(long name, long* data, long length)
{
  long null = 0;
  writeOneLong(null);
  writeOneLong(name);
  writeOneLong(length);
  for(long i = 0; i < length; ++i){
    writeOneLong(data[i]);
  }
}

long Hub::readOneLong()
{
  return (this->read()) |
         (this->read() << 8) |
         (this->read() << 16) |
         (this->read() << 24);
}

void Hub::writeOneLong(long value)
{
  Serial.write((char*)&value, 4);
}

long Hub::read(){
  return Serial.read();
}
