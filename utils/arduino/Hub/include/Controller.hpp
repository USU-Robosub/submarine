#ifndef CONTROLLER
#define CONTROLLER

#include <Emitter.hpp>

enum class ControllerType{
  BASIC,
  FREEZABLE
};

class Controller {
public:
  virtual void execute(Emitter* emitter, int32_t* data, int32_t length) = 0;
  virtual ControllerType type() {
    return ControllerType::BASIC;
  }
};

#endif
