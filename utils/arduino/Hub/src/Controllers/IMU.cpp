#include <Controllers/IMU.hpp>
#include <Calibration/Magnetic.hpp>

Controllers::IMU::IMU(unsigned long timeBetweenSamples,
  int32_t handler,
  Components::Sensors::Magnetometer* magnetometer,
  Components::Sensors::Gyroscope* gyroscope,
  Components::Sensors::Accelerometer* accelerometer)
  : enabled(false),
    handler(handler),
    timeBetweenSamples(timeBetweenSamples),
    lastSampleTime(0),
    magnetometer(magnetometer),
    gyroscope(gyroscope),
    accelerometer(accelerometer){

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

void Controllers::IMU::update(Calibration::Magnetic::Model model){
  if(this->enabled){
    unsigned long now = millis();
    if(this->lastSampleTime + this->timeBetweenSamples < now){
      this->lastSampleTime = now;

      auto gyro = this->gyroscope->measureAngularVelocity();
      auto accel = this->accelerometer->measureLinearAcceleration();
      auto magnet = this->magnetometer->measureMagneticField();

      Calibration::Magnetic magnetCalibration;

      auto fixedMagnet = magnetCalibration.applyCalibration(model, {magnet.z, magnet.y, magnet.z});

      int32_t data[18] = {
        gyro.x, gyro.y, gyro.z,
        accel.x, accel.y, accel.z,
        fixedMagnet.x, fixedMagnet.y, fixedMagnet.z,
        magnet.x, magnet.y, magnet.z,
        model.hardIronOffset[0], model.hardIronOffset[1], model.hardIronOffset[2],
        model.scale[0], model.scale[1], model.scale[2]
      };

      this->emitter->emit(this->handler, data, 18);
    }
  }
}

void Controllers::IMU::use(Emitter* emitter){
  this->emitter = emitter;
}
