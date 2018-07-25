#ifndef COMPONENTS_SENSORS_LOW_POWER_SENSOR
#define COMPONENTS_SENSORS_LOW_POWER_SENSOR

#include <Arduino.h>

namespace Components{
  namespace Sensors{
    class LowPowerSensor;
  }
}

class Components::Sensors::LowPowerSensor{
public:
  virtual void enable() = 0;
  virtual void sleep() = 0;
};

#endif
