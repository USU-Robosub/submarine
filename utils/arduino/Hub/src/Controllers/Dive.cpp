#include <Controllers/Dive.hpp>
#include <tools.hpp>

Controllers::Dive::Dive(Components::Motors::Motor* front, Components::Motors::Motor* back)
  : frontMotor(front),
    backMotor(back){

}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  (void) hub;
  if(length == 1){
    this->frontMotor->power(int32AsFloat(data[0]));
    this->backMotor->power(int32AsFloat(data[0]));
  } else if(length == 2){
    this->frontMotor->power(int32AsFloat(data[0]) + int32AsFloat(data[1]));
    this->backMotor->power(int32AsFloat(data[0]) - int32AsFloat(data[1]));
  }
}

void Controllers::Dive::freeze(){
  this->frontMotor->disable();
  this->backMotor->disable();
}

void Controllers::Dive::unfreeze(){
  this->frontMotor->enable();
  this->backMotor->enable();
}
