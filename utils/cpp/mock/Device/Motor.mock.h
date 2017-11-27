#ifndef MOCK_DEVICE_MOTOR
#define MOCK_DEVICE_MOTOR

#include "Device/Motor.h"

namespace Device{
  namespace Mock{
    class TestMotor : public Motor {
    public:
      void setSpeed(double speed){
        didSetSpeed = true;
      }
      bool didSetSpeed = false;
    };
  }
}

#endif
