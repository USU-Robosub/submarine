#ifndef COMPONENTS_SENSORS_MAGNETOMETER
#define COMPONENTS_SENSORS_MAGNETOMETER

#include <Arduino.h>

namespace Components{
  namespace Sensors{
    class Magnetometer;
  }
}

class Components::Sensors::Magnetometer{
public:
  virtual int32_t x() = 0;
  virtual int32_t y() = 0;
  virtual int32_t z() = 0;
};

#endif
