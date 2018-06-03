#ifndef EMITTER_MOCK
#define EMITTER_MOCK

#include <Controller.hpp>

namespace Mock{
  class Controller;
}

class Mock::Controller : public ::Controller{
public:
  Controller(int32_t* buffer);
  void execute(Emitter* emitter, int32_t* data, int32_t length);
  void kill(){
    // TODO not implemented
  }
  void restart(){
    // TODO not implemented
  }

private:
  int32_t* buffer;
};

Mock::Controller::Controller(int32_t* buffer)
  : buffer(buffer) {}

void Mock::Controller::execute(Emitter* emitter, int32_t* data, int32_t length){
  for(int32_t i = 0; i < length; ++i){
    this->buffer[i] = data[i];
  }
}

#endif
