#ifndef COMPONENTS_SENSORS_MPU6050
#define COMPONENTS_SENSORS_MPU6050

#include <Components/Sensors/Accelerometer.hpp>
#include <Components/Sensors/Gyroscope.hpp>
#include <Components/Sensors/Thermometer.hpp>

namespace Components{
  namespace Sensors{
    class MPU6050;
  }
}

#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C

// full scale gyro ranges
#define GYRO_FSR_250 0 // SSF: 131 (LSB / (deg / sec))
#define GYRO_FSR_500 1 // SSF: 65.5
#define GYRO_FSR_1000 2 // SSF: 32.8
#define GYRO_FSR_2000 3 // SSF: 16.4

// full scale accel ranges
#define ACCEL_FSR_2 0 // SSF: 16384 (LSB / g)
#define ACCEL_FSR_4 1 // SSF: 8192
#define ACCEL_FSR_8 2 // SSF: 4096
#define ACCEL_FSR_16 3 // SSF: 2048

class Components::Sensors::MPU6050 : public Components::Sensors::Accelerometer, public Components::Sensors::Gyroscope , public Components::Sensors::Thermometer{
public:
  MPU6050(unsigned long minSampleTimeDelta);
  Components::Sensors::ThreeAxisMeasurement<MicroradianPerSecond> measureAngularVelocity();
  Components::Sensors::ThreeAxisMeasurement<MicrometerPerSecondSquared> measureLinearAcceleration();
  Millicelcius measureTemperature();
  void setGyroFullScaleRange(uint8_t mode);
  void setAccelFullScaleRange(uint8_t mode);

private:
  void measure();

  // (0.01745329252 (deg -> rad) * 1000 * 1000) / SSF = SSF_URAD
  const int16_t GYRO_SSF_URAD[4] = { 133, 266, 532, 1064 }; // (urad / s) / LSB
  // (9.80665 (g -> m/s^2) * 1000 * 1000) / SSF = SSF_UMS2
  const int16_t ACCEL_SSF_UMS2[4] = { 599, 1197, 2394, 4788 }; // ( um / s^2 ) / LSB

  unsigned long minSampleTimeDelta;
  unsigned long lastSampleTime;
  MicroradianPerSecond gyroX;
  MicroradianPerSecond gyroY;
  MicroradianPerSecond gyroZ;
  MicrometerPerSecondSquared accelX;
  MicrometerPerSecondSquared accelY;
  MicrometerPerSecondSquared accelZ;
  Millicelcius temp;
  uint8_t gyroScaleMode;
  uint8_t accelScaleMode;
};

#endif
