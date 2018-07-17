#ifndef COMPONENTS_SENSORS_MAGNETOMETER
#define COMPONENTS_SENSORS_MAGNETOMETER

namespace Components{
  namespace Sensors{
    class Magnetometer;
  }
}

class Components::Sensors::Magnetometer{
public:
  virtual int x() = 0;
  virtual int y() = 0;
  virtual int z() = 0;
};

#endif
