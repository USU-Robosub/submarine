#include <Controllers/Dive.hpp>

Controllers::Dive::Dive(int frontPin, int backPin, bool protectMotors)
  : frontPin(frontPin),
    backPin(backPin),
    protectMotors(protectMotors),
    stopped(true) {
  this->front.attach(frontPin);
  this->back.attach(backPin);
  this->front.write(90);
  this->back.write(90);
}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 1 && !stopped){
    this->front.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    this->back.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
  }
}

void Controllers::Dive::freeze(){
  stopped = true;
  this->front.write(90);
  this->back.write(90);
  this->front.detach(); // this should work
  this->back.detach();
}

void Controllers::Dive::unfreeze(){
  stopped = false;
  this->front.attach(this->frontPin);
  this->back.attach(this->backPin);
}
