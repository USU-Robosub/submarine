#include <Components/Sensors/HMC5883L.hpp>

#include <Wire.h>

#define CHIP_ADDRESS 0x1E //0011110b, I2C 7bit address of HMC5883

Components::Sensors::HMC5883L::HMC5883L(unsigned long minSampleTimeDelta)
  : minSampleTimeDelta(minSampleTimeDelta),
    lastSampleTime(0),
    x(0),
    y(0),
    z(0){
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(CHIP_ADDRESS); //open communication with HMC5883
  Wire.send(0x02); //select mode register
  Wire.send(0x00); //continuous measurement mode
  Wire.endTransmission();
}

Components::Sensors::ThreeAxisMeasurement<Nanotesla> Components::Sensors::HMC5883L::measureMagneticField(){
  return {this->x, this->y, this->z};
}

void Components::Sensors::HMC5883L::measure(){
  unsigned long now = millis();
  if(this->lastSampleTime + this->minSampleTimeDelta < now){
    this->lastSampleTime = now;

    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(CHIP_ADDRESS);
    Wire.send(0x03); //select register 3, X MSB register
    Wire.endTransmission();

   //Read data from each axis, 2 registers per axis
    Wire.requestFrom(CHIP_ADDRESS, 6);
    if(6<=Wire.available()){
      int16_t tmpX = 0;
      int16_t tmpY = 0;
      int16_t tmpZ = 0;
      tmpX = Wire.receive()<<8; //X msb
      tmpX |= Wire.receive(); //X lsb
      tmpZ = Wire.receive()<<8; //Z msb
      tmpZ |= Wire.receive(); //Z lsb
      tmpY = Wire.receive()<<8; //Y msb
      tmpY |= Wire.receive(); //Y lsb
      this->x = tmpX;
      this->y = tmpY;
      this->z = tmpZ;
    }
  }
}