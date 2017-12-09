#ifndef OBJECT_POSEIDON
#define OBJECT_POSEIDON

#include "Device/Thruster.h"

namespace Object{
  class Poseidon{
  public:
    struct Config{
      Device::Thruster frontThruster;
    };
  };
}

#endif
