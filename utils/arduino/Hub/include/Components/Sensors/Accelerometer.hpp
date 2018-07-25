#ifndef COMPONENTS_SENSORS_ACCELEROMETER
#define COMPONENTS_SENSORS_ACCELEROMETER

#include <Components/Sensors/ThreeAxisSensor.hpp>
#include <Components/Sensors/units.hpp>

namespace Components{
  namespace Sensors{
    class Accelerometer;
  }
}

class Components::Sensors::Accelerometer{
public:
  virtual Components::Sensors::ThreeAxisMeasurement<MicrometerPerSecondSquared> measureLinearAcceleration() = 0;
};

#endif
