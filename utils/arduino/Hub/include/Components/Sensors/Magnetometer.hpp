#ifndef COMPONENTS_SENSORS_MAGNETOMETER
#define COMPONENTS_SENSORS_MAGNETOMETER

#include <Components/Sensors/ThreeAxisSensor.hpp>
#include <Components/Sensors/units.hpp>

namespace Components{
  namespace Sensors{
    class Magnetometer;
  }
}

class Components::Sensors::Magnetometer{
public:
  virtual Components::Sensors::ThreeAxisMeasurement<Nanotesla> measureMagneticField() = 0;
};

#endif
