#include <Controllers/Dive.hpp>

Controllers::Dive::Dive(int frontPin, int backPin, bool protectMotors)
  : frontPin(frontPin),
    backPin(backPin),
    protectMotors(protectMotors) {
  this->front.attach(frontPin);
  this->back.attach(backPin);
  this->front.write(90);
  this->back.write(90);
}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 1){
    this->front.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    this->back.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
  }
}

void Controllers::Dive::kill(){
  this->front.detach();
  this->back.detach();
}

void Controllers::Dive::restart(){
  this->front.attach(this->frontPin);
  this->back.attach(this->backPin);
}