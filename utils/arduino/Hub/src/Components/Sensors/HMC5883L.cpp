#include <Components/Sensors/HMC5883L.hpp>

#include <Wire.h>

#define CHIP_ADDRESS 0x1E //0011110b, I2C 7bit address of HMC5883

Components::Sensors::HMC5883L::HMC5883L()
  : savedX(0), 
    savedY(0), 
    savedZ(0){
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(CHIP_ADDRESS); //open communication with HMC5883
  Wire.send(0x02); //select mode register
  Wire.send(0x00); //continuous measurement mode
  Wire.endTransmission();
}

int32_t Components::Sensors::HMC5883L::x(){
  return this->savedX;
}

int32_t Components::Sensors::HMC5883L::y(){
  return this->savedY;
}

int32_t Components::Sensors::HMC5883L::z(){
  return this->savedZ;
}

void Components::Sensors::HMC5883L::update(){
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
    this->savedX = tmpX;
    this->savedY = tmpY;
    this->savedZ = tmpZ;
  }
}
