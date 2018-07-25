#ifndef COMPONENTS_SENSORS_THERMOMETER
#define COMPONENTS_SENSORS_THERMOMETER

#include <Components/Sensors/units.hpp>

namespace Components{
  namespace Sensors{
    class Thermometer;
  }
}

class Components::Sensors::Thermometer{
public:
  virtual Millicelcius measureTemperature() = 0;
};

#endif
