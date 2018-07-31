#include <Components/Sensors/BMP085.hpp>
#include <Log.hpp>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

Components::Sensors::BMP085::BMP085(unsigned long minSampleTimeDelta)
  : lastSampleTime(0),
    minSampleTimeDelta(minSampleTimeDelta),
    _pressure(0),
    _temp(0),
    OSS(0),
    p0(101325){
  this->bmp085Calibration();
}

Pascal Components::Sensors::BMP085::measurePressure(){
  this->measure();
  return this->_pressure;
}

Millicelcius Components::Sensors::BMP085::measureTemperature(){
  this->measure();
  return this->_temp;
}

void Components::Sensors::BMP085::measure(){
  unsigned long now = millis();
  
  if(this->lastSampleTime + this->minSampleTimeDelta < now){
    this->lastSampleTime = now;
    
    // this->_temp = this->bmp085ReadUT();
    // this->_pressure = this->bmp085ReadUP();
    
    this->_temp = this->bmp085GetTemperature(this->bmp085ReadUT());
    this->_pressure = this->bmp085GetPressure(this->bmp085ReadUP());
  }
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
int64_t Components::Sensors::BMP085::bmp085GetTemperature(int64_t ut)
{
  int64_t x1, x2;
  
  x1 = (((int64_t)ut - (int64_t)ac6)*(int64_t)ac5) >> 15;
  x2 = ((int64_t)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
int64_t Components::Sensors::BMP085::bmp085GetPressure(int64_t up)
{
  int64_t x1, x2, x3, b3, b6, p;
  uint64_t b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((int64_t)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (uint64_t)(x3 + 32768))>>15;
  
  b7 = ((uint64_t)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

int64_t Components::Sensors::BMP085::bmp085ReadUT()
{
  int64_t ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
int64_t Components::Sensors::BMP085::bmp085ReadUP()
{
  uint8_t msb, lsb, xlsb;
  int64_t up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((int64_t) msb << 16) | ((int64_t) lsb << 8) | (int64_t) xlsb) >> (8-OSS);
  
  return up;
}




// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void Components::Sensors::BMP085::bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Read 1 byte from the BMP085 at 'address'
uint8_t Components::Sensors::BMP085::bmp085Read(uint8_t address)
{
  int8_t data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
uint16_t Components::Sensors::BMP085::bmp085ReadInt(uint8_t address)
{
  uint8_t msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int16_t) msb<<8 | lsb;
}
