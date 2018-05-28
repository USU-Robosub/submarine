#ifndef FREEZABLE_CONTROLLER
#define FREEZABLE_CONTROLLER

#include <Controller.hpp>
#include <Freezable.hpp>

class FreezableController: public Controller, public Freezable {
public:
  virtual void execute(Emitter* emitter, int32_t* data, int32_t length) = 0;
  ControllerType type() {
    return ControllerType::FREEZABLE;
  }
  virtual void freeze() = 0;
  virtual void unfreeze() = 0;
};

#endif
