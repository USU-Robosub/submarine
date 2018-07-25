#ifndef COMPONENTS_SENSORS_GYROSCOPE
#define COMPONENTS_SENSORS_GYROSCOPE

#include <Components/Sensors/ThreeAxisSensor.hpp>
#include <Components/Sensors/units.hpp>

namespace Components{
  namespace Sensors{
    class Gyroscope;
  }
}

class Components::Sensors::Gyroscope{
public:
  virtual Components::Sensors::ThreeAxisMeasurement<MicroradianPerSecond> measureAngularVelocity() = 0;
};

#endif
