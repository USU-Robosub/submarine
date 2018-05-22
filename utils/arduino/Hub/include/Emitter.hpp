#ifndef EMITTER
#define EMITTER

#include <Arduino.h>

class Emitter {
public:
  virtual void emit(int32_t name, int32_t* data, int32_t length) = 0;
  virtual void kill() = 0;
  virtual void restart() = 0;
};

#endif
