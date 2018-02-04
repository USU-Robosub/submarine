#ifndef EMITTER
#define EMITTER

class Emitter {
public:
  virtual void emit(long name, long* data, long length) = 0;
};

#endif
