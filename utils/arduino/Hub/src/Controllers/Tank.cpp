#include <Controllers/Tank.hpp>
#include <tools.hpp>

Controllers::Tank::Tank(Components::Motors::Motor* left, Components::Motors::Motor* right)
  : leftMotor(left),
    rightMotor(right){

}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2){
    this->leftMotor->power(int32AsFloat(data[0]) + int32AsFloat(data[1]));
    this->rightMotor->power(int32AsFloat(data[0]) - int32AsFloat(data[1]));
  }
}

void Controllers::Tank::freeze(){
  this->leftMotor->disable();
  this->rightMotor->disable();
}

void Controllers::Tank::unfreeze(){
  this->leftMotor->enable();
  this->rightMotor->enable();
}
