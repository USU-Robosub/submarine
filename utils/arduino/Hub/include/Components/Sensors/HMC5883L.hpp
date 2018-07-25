#ifndef COMPONENTS_SENSORS_HMC5883L
#define COMPONENTS_SENSORS_HMC5883L

#include <Components/Sensors/Magnetometer.hpp>

namespace Components{
  namespace Sensors{
    class HMC5883L;
  }
}

class Components::Sensors::HMC5883L : public Components::Sensors::Magnetometer{
public:
  HMC5883L(unsigned long minSampleTimeDelta);
  Components::Sensors::ThreeAxisMeasurement<Nanotesla> measureMagneticField();

private:
  void measure();

  unsigned long minSampleTimeDelta;
  unsigned long lastSampleTime;
  Nanotesla x;
  Nanotesla y;
  Nanotesla z;
};

#endif
