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
  short bmp085GetTemperature(unsigned int ut);
  long bmp085GetPressure(unsigned long up);
  unsigned int bmp085ReadUT();
  unsigned long bmp085ReadUP();
  void bmp085Calibration();
  char bmp085Read(unsigned char address);
  int bmp085ReadInt(unsigned char address);
  
  unsigned long lastSampleTime;
  unsigned long minSampleTimeDelta;
  Pascal _pressure;
  Millicelcius _temp;
  
  
  const unsigned char OSS;  // Oversampling Setting
  
  // Calibration values
  int ac1;
  int ac2; 
  int ac3; 
  unsigned int ac4;
  unsigned int ac5;
  unsigned int ac6;
  int b1; 
  int b2;
  int mb;
  int mc;
  int md;
  
  // b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
  // so ...Temperature(...) must be called before ...Pressure(...).
  long b5; 
  
  short temperature;
  long pressure;
  
  // Use these for altitude conversions
  const float p0;     // Pressure at sea level (Pa)
  float altitude;
};

#endif
