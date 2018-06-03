#ifndef EMITTER_MOCK
#define EMITTER_MOCK

#include <Emitter.hpp>

namespace Mock{
  class Emitter;
}

class Mock::Emitter : public ::Emitter{
public:
  Emitter(int32_t* buffer)
    : buffer(buffer) {}

  void emit(int32_t name, int32_t* data, int32_t length){
    for(int32_t i = 0; i < length; ++i){
      this->buffer[i] = data[i];
    }
  }

  void kill(){
    isKilled = true;
    stateChanges++;
  }

  void restart(){
    isKilled = false;
    stateChanges++;
  }

  bool $isKilled(){
    return isKilled;
  }

  int $stateChanges(){
    return stateChanges;
  }

  int $makeActive(){
    isKilled = false;
  }

private:
  int stateChanges = 0;
  bool isKilled = true;
  int32_t* buffer;
};

#endif
