#include "Device/Motor.h"

observable<double> Device::Motor::getDevicePower(observable<double> power, Math::Range operatingRange){
  return power.map([=](double power) {
    return Math::bound(Math::map(power, Device::Motor::powerRange, operatingRange), operatingRange);
  });
}
