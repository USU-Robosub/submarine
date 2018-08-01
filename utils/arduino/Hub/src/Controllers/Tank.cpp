#include <Controllers/Tank.hpp>
#include <tools.hpp>
#include <Log.hpp>

Controllers::Tank::Tank(Components::Motors::Motor* left, Components::Motors::Motor* right)
  : leftMotor(left),
    rightMotor(right){

}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  (void) hub;
  if(length == 2){
    this->leftMotor->power(int32AsFloat(data[0]) + int32AsFloat(data[1]));
    this->rightMotor->power(int32AsFloat(data[0]) - int32AsFloat(data[1]));
    VERBOSE("Tank Power,Steer",2,(int)(int32AsFloat(data[0])*1000),(int)(int32AsFloat(data[1])*1000));
  }
}

void Controllers::Tank::freeze(){
  this->leftMotor->disable();
  this->rightMotor->disable();
  VERBOSE("Tank Enable",1,0);
}

void Controllers::Tank::unfreeze(){
  this->leftMotor->enable();
  this->rightMotor->enable();
  VERBOSE("Tank Enable",1,1);
}
