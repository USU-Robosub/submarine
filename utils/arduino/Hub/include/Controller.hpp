#ifndef CONTROLLER
#define CONTROLLER

#include <Emitter.hpp>

class Controller {
public:
  virtual void execute(Emitter* emitter, int32_t* data, int32_t length) = 0;
};

#endif
