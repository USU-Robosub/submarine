#ifndef MOCK_DEVICE_MOTOR
#define MOCK_DEVICE_MOTOR

#include "Device/Motor.h"

namespace Device{
  namespace Mock{
    class TestMotor{
    public:
      void setPower(double power){
        didSetPower = true;
      }
      bool didSetPower = false;
    };
  }
}

#endif
