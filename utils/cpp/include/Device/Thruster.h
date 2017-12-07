#ifndef DEVICE_THRUSTER
#define DEVICE_THRUSTER

#include <algorithm>
#include "Device/Motor.h"
#include "Comm/Hub.h"

namespace Device{
  class Thruster : public Motor{
  public:
    Thruster(Comm::Hub* hub, unsigned int id) : hub(hub), id(id) {}
    void setPower(double power);
    void enable();
    void disable();
  private:
    bool isEnabled = false;
    Comm::Hub* hub;
    unsigned int id;
  };
}

#endif
