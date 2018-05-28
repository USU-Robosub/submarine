#include <Controllers/Tank.hpp>

Controllers::Tank::Tank(int leftPin, int rightPin, bool protectMotors)
  : left(),
    right(),
    protectMotors(protectMotors),
    stopped(true),
    leftPin(leftPin),
    rightPin(rightPin){
  this->left.attach(leftPin);
  this->right.attach(rightPin);
  this->left.write(90);
  this->right.write(90);
}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2 && !stopped){
    int mixLeft =  data[0] + (data[1] - 90);
    int mixRight =  data[0] - (data[1] - 90);
    this->left.write(this->protectMotors ? constrain(mixLeft, 10, 170) : mixLeft);
    this->right.write(this->protectMotors ? constrain(mixRight, 10, 170) : mixRight);
  }
}

void Controllers::Tank::freeze(){
  stopped = true;
  this->left.write(90);
  this->right.write(90);
  this->left.detach();
  this->right.detach();
}

void Controllers::Tank::unfreeze(){
  stopped = false;
  this->left.attach(this->leftPin);
  this->right.attach(this->rightPin);
}
