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
  HMC5883L();
  int32_t x();
  int32_t y();
  int32_t z();
  void update();
  
private:
  int32_t savedX;
  int32_t savedY;
  int32_t savedZ;
};

#endif
