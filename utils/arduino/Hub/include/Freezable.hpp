#ifndef FREEZABLE_CLASS
#define FREEZABLE_CLASS

#include <Arduino.h>

#include <iostream>

class Freezable {
public:
  virtual void freeze() = 0;
  virtual void unfreeze() = 0;
  virtual void test() = 0;
};

#endif
