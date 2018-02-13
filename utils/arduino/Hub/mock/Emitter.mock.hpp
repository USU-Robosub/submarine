#ifndef EMITTER_MOCK
#define EMITTER_MOCK

#include <Emitter.hpp>

namespace Mock{
  class Emitter;
}

class Mock::Emitter : public ::Emitter{
public:
  Emitter(int32_t* buffer);
  void emit(int32_t name, int32_t* data, int32_t length);

private:
  int32_t* buffer;
};

Mock::Emitter::Emitter(int32_t* buffer)
  : buffer(buffer) {}

void Mock::Emitter::emit(int32_t name, int32_t* data, int32_t length){
  for(int32_t i = 0; i < length; ++i){
    this->buffer[i] = data[i];
  }
}

#endif
