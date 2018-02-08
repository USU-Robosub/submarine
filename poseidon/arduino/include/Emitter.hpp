#ifndef EMITTER
#define EMITTER

#ifndef UNIT_TESTS
  #include <Arduino.h>
#else
  #include <cstdint>
#endif

class Emitter {
public:
  virtual void emit(int32_t name, int32_t* data, int32_t length) = 0;
};

#endif
