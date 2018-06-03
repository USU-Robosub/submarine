#ifndef STRING_EMITTER
#define STRING_EMITTER

#include <Emitter.hpp>

class StringEmitter : public Emitter { // prints anything emitted to serial as a human readable string
public:
  StringEmitter(char*(*formatter)(int32_t name, int32_t data, int32_t length));
  void emit(int32_t name, int32_t* data, int32_t length);
};

#endif
