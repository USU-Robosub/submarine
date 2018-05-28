#ifndef FREEZABLE_CLASS
#define FREEZABLE_CLASS

#include <Arduino.h>

class Freezable {
public:
  virtual void freeze() = 0;
  virtual void unfreeze() = 0;
};

#endif
