#include <Controllers/IMU.hpp>
#include <Calibration/Magnetic.hpp>
#include <Log.hpp>

Controllers::IMU::IMU(unsigned long timeBetweenSamples,
  int32_t handler,
  Components::Sensors::Magnetometer* magnetometer,
  Components::Sensors::Gyroscope* gyroscope,
  Components::Sensors::Accelerometer* accelerometer,
  Components::Sensors::Piezometer* piezometer,
  Components::Sensors::Thermometer* thermometer)
  : enabled(false),
    handler(handler),
    timeBetweenSamples(timeBetweenSamples),
    lastSampleTime(0),
    magnetometer(magnetometer),
    gyroscope(gyroscope),
    accelerometer(accelerometer),
    piezometer(piezometer),
    thermometer(thermometer){

}

void Controllers::IMU::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 1){
    if(data[0] == 0){
      this->enabled = false;
    }else{
      this->enabled = true;
    }
  }
}

void Controllers::IMU::update(){
  if(this->enabled){
    unsigned long now = millis();
    if(this->lastSampleTime + this->timeBetweenSamples < now){
      this->lastSampleTime = now;

      auto gyro = this->gyroscope->measureAngularVelocity();
      auto accel = this->accelerometer->measureLinearAcceleration();
      auto magnet = this->magnetometer->measureMagneticField();
      auto pressure = this->piezometer->measurePressure();
      auto temp = this->thermometer->measureTemperature();
      int32_t data[11] = {
        gyro.x, gyro.y, gyro.z,
        accel.x, accel.y, accel.z,
        magnet.x, magnet.y, magnet.z,
        pressure,
        temp
      };

      this->emitter->emit(this->handler, data, 11);
    }
  }
}

void Controllers::IMU::use(Emitter* emitter){
  this->emitter = emitter;
}
