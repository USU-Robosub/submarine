#include <Controllers/Tank.hpp>

Controllers::Tank::Tank(int leftPin, int rightPin, bool protectMotors)
  : protectMotors(protectMotors) {
  this->leftPin = leftPin;
  this->rightPin = rightPin;
  this->left.attach(leftPin);
  this->right.attach(rightPin);
}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2){
    int mixLeft =  data[0] + (data[1] - 90);
    int mixRight =  data[0] - (data[1] - 90);
    this->left.write(this->protectMotors ? constrain(mixLeft, 10, 170) : mixLeft);
    this->right.write(this->protectMotors ? constrain(mixRight, 10, 170) : mixRight);
  }
}

void  Controllers::Tank::kill(){
   this->left.detach();
   this->right.detach();
}

void  Controllers::Tank::restart(){
  this->left.attach(this->leftPin);
  this->right.attach(this->rightPin);
}