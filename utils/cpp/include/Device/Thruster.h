#ifndef DEVICE_THRUSTER
#define DEVICE_THRUSTER

#include <algorithm>
#include "Device/Motor.h"
#include "Comm/Hub.h"

namespace Device{
  class Thruster : public Motor{
  public:
    Thruster(Comm::Hub* hub) : hub(hub) {}
    void setPower(double power);
  private:
    Comm::Hub* hub;
  };
}

#endif
