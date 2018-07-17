#ifndef COMPONENTS_SENSORS_ACCELEROMETER
#define COMPONENTS_SENSORS_ACCELEROMETER

namespace Components{
  namespace Sensors{
    class Accelerometer;
  }
}

class Components::Sensors::Accelerometer{
public:
  virtual int x() = 0;
  virtual int y() = 0;
  virtual int z() = 0;
};

#endif
