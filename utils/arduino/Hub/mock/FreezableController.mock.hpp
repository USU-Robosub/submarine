#ifndef FREEZABLE_CONTROLLER_MOCK
#define FREEZABLE_CONTROLLER_MOCK

#include <FreezableController.hpp>

namespace Mock{
  class FreezableController;
}

class Mock::FreezableController : public ::FreezableController{
public:
  FreezableController(int32_t* buffer)
    : buffer(buffer) {}

  void execute(Emitter* emitter, int32_t* data, int32_t length){
    for(int32_t i = 0; i < length; ++i){
      this->buffer[i] = data[i];
    }
  }

  void freeze(){
    isFrozen = true;
  }

  void unfreeze(){
    isFrozen = false;
  }

  bool $isFrozen(){
    return isFrozen;
  }

private:
  bool isFrozen = true;
  int32_t* buffer;
};

#endif
