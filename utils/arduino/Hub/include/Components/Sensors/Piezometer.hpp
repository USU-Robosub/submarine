#ifndef COMPONENTS_SENSORS_PIEZOMETER
#define COMPONENTS_SENSORS_PIEZOMETER

#include <Components/Sensors/units.hpp>

namespace Components{
  namespace Sensors{
    class Piezometer;
  }
}

class Components::Sensors::Piezometer{
public:
  virtual Kilopascal measurePressure() = 0;
};

#endif
