#ifndef CONTROLLER
#define CONTROLLER

#include <Emitter.hpp>

class Controller {
public:
  virtual void execute(Emitter* emitter, long* data, long length) = 0;
};

#endif
