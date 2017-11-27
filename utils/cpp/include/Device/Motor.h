#ifndef DEVICE_MOTOR
#define DEVICE_MOTOR

namespace Device{
  class Motor{
  public:
    virtual void setPower(double speed) = 0;
  };
}

#endif
