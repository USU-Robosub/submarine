#include <Components/Sensors/MPU6050.hpp>

#include <Arduino.h>
#include <Wire.h>

#define CHIP_ADDRESS 0x68

Components::Sensors::MPU6050::MPU6050(unsigned long minSampleTimeDelta)
  : minSampleTimeDelta(minSampleTimeDelta),
    lastSampleTime(0),
    gyroX(0),
    gyroY(0),
    gyroZ(0),
    accelX(0),
    accelY(0),
    accelZ(0),
    temp(0),
    gyroScaleMode(0) {
  // enable sensor
  Wire.beginTransmission(CHIP_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

Components::Sensors::ThreeAxisMeasurement<MicroradianPerSecond> Components::Sensors::MPU6050::measureAngularVelocity(){
  this->measure();
  return {this->gyroX, this->gyroY, this->gyroZ};
}

Components::Sensors::ThreeAxisMeasurement<MicrometerPerSecondSquared> Components::Sensors::MPU6050::measureLinearAcceleration(){
  this->measure();
  return {this->accelX, this->accelY, this->accelZ};
}

Millicelcius Components::Sensors::MPU6050::measureTemperature(){
  this->measure();
  return 0; // TODO ignore temp for now
}

void Components::Sensors::MPU6050::measure(){
  unsigned long now = millis();
  if(this->lastSampleTime + this->minSampleTimeDelta < now){
    this->lastSampleTime = now;
    // request data
    Wire.beginTransmission(CHIP_ADDRESS);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(CHIP_ADDRESS,14,true);  // request a total of 14 registers
    // read data
    int16_t tmpAccelX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    int16_t tmpAccelY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    int16_t tmpAccelZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    int16_t tmpTemp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    int16_t tmpGyroX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    int16_t tmpGyroY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    int16_t tmpGyroZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    // convert units
    // convert from raw 16-bit gyro sensor value in scaled degrees per second to microradians per second
    int32_t gyroScaleFactor = this->GYRO_SSF_URAD[this->gyroScaleMode];
    this->gyroX = static_cast<int32_t>(tmpGyroX) * gyroScaleFactor;
    this->gyroY = static_cast<int32_t>(tmpGyroY) * gyroScaleFactor;
    this->gyroZ = static_cast<int32_t>(tmpGyroZ) * gyroScaleFactor;
    // convert from raw 16-bit accel sensor value in scaled g to micrometers per second
    int32_t accelScaleFactor = this->ACCEL_SSF_UMS2[this->accelScaleMode];
    this->accelX = static_cast<int32_t>(tmpAccelX) * accelScaleFactor;
    this->accelY = static_cast<int32_t>(tmpAccelY) * accelScaleFactor;
    this->accelZ = static_cast<int32_t>(tmpAccelZ) * accelScaleFactor;
    // TODO ignore temp for now
  }
}

void Components::Sensors::MPU6050::setGyroFullScaleRange(uint8_t mode){
  this->gyroScaleMode = mode;
  Wire.beginTransmission(CHIP_ADDRESS);
  Wire.write(GYRO_CONFIG);
  Wire.write(mode << 3);
  Wire.endTransmission(true);
}

void Components::Sensors::MPU6050::setAccelFullScaleRange(uint8_t mode){
  this->accelScaleMode = mode;
  Wire.beginTransmission(CHIP_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(mode << 3);
  Wire.endTransmission(true);
}
