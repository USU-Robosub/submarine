#ifndef COMPONENTS_SENSORS_BMP085
#define COMPONENTS_SENSORS_BMP085

#include <Components/Sensors/Piezometer.hpp>
#include <Components/Sensors/Thermometer.hpp>
#include <Wire.h>

namespace Components{
  namespace Sensors{
    class BMP085;
  }
}

class Components::Sensors::BMP085 : public Components::Sensors::Piezometer, public Components::Sensors::Thermometer{
public:
  BMP085(unsigned long minSampleTimeDelta);
  Pascal measurePressure();
  Millicelcius measureTemperature();

private:
  void measure();
  int64_t bmp085GetTemperature(int64_t ut);
  int64_t bmp085GetPressure(int64_t up);
  int64_t bmp085ReadUT();
  int64_t bmp085ReadUP();
  void bmp085Calibration();
  uint8_t bmp085Read(unsigned char address);
  uint16_t bmp085ReadInt(unsigned char address);
  
  unsigned long lastSampleTime;
  unsigned long minSampleTimeDelta;
  Pascal _pressure;
  Millicelcius _temp;
  
  
  const uint8_t OSS;  // Oversampling Setting
  
  // Calibration values
  int16_t ac1;
  int16_t ac2; 
  int16_t ac3; 
  uint16_t ac4;
  uint16_t ac5;
  uint16_t ac6;
  int16_t b1; 
  int16_t b2;
  int16_t mb;
  int16_t mc;
  int16_t md;
  
  // b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
  // so ...Temperature(...) must be called before ...Pressure(...).
  int64_t b5; 
  
  int64_t temperature;
  int64_t pressure;
  
  // Use these for altitude conversions
  const float p0;     // Pressure at sea level (Pa)
  float altitude;
};

#endif
