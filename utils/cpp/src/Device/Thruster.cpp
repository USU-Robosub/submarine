#include "Device/Thruster.h"

void Device::Thruster::setPower(double power){
  if(this->isEnabled){
    this->hub->lock();
    this->hub->writeInt(this->id);
    this->hub->writeInt(std::min(std::max((int)((power + 1) * 200), 0), 400));
    this->hub->unlock();
  }
}

void Device::Thruster::enable(){
  this->isEnabled = true;
}

void Device::Thruster::disable(){
  this->isEnabled = false;
}