#ifndef COMPONENTS_SENSORS_THREE_AXIS_SENSOR
#define COMPONENTS_SENSORS_THREE_AXIS_SENSOR

#include <Arduino.h>

namespace Components{
  namespace Sensors{
    struct ThreeAxisMeasurement;
  }
}

template <typename T>
struct Components::Sensors::ThreeAxisMeasurement{
  T x;
  T y;
  T z;
};

#endif
