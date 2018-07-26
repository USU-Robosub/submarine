#ifndef CONTROLLERS_IMU
#define CONTROLLERS_IMU

#include <Emitter.hpp>
#include <Controller.hpp>
#include <Components/Sensors/Magnetometer.hpp>
#include <Components/Sensors/Gyroscope.hpp>
#include <Components/Sensors/Accelerometer.hpp>
#include <Calibration/Magnetic.hpp>

namespace Controllers{
  class IMU;
}

class Controllers::IMU : public Controller{
public:
  IMU(unsigned long timeBetweenSamples,
    int32_t handler,
    Components::Sensors::Magnetometer* magnetometer,
    Components::Sensors::Gyroscope* gyroscope,
    Components::Sensors::Accelerometer* accelerometer);
  void execute(Emitter* hub, int32_t* data, int32_t length);
  void update(Calibration::Magnetic::Model model);
  void use(Emitter* emitter);

private:
  bool enabled;
  int32_t handler;
  unsigned long timeBetweenSamples;
  unsigned long lastSampleTime;
  Components::Sensors::Magnetometer* magnetometer;
  Components::Sensors::Gyroscope* gyroscope;
  Components::Sensors::Accelerometer* accelerometer;
  Emitter* emitter;
};

#endif
