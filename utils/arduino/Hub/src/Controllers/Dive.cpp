#include <Controllers/Dive.hpp>
#include <tools.hpp>
#include <Log.hpp>

Controllers::Dive::Dive(Components::Motors::Motor* front, Components::Motors::Motor* back)
  : frontMotor(front),
    backMotor(back){

}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  (void) hub;
  if(length == 1){
    // VERBOSE("Dive Power,Steer",2,(int)(int32AsFloat(data[0])*1000),0);
    this->frontMotor->power(int32AsFloat(data[0]));
    this->backMotor->power(int32AsFloat(data[0]));
  } else if(length == 2){
    // VERBOSE("Dive Power,Steer",2,(int)(int32AsFloat(data[0])*1000),(int)(int32AsFloat(data[1])*1000));
    this->frontMotor->power(int32AsFloat(data[0]) + int32AsFloat(data[1]));
    this->backMotor->power(int32AsFloat(data[0]) - int32AsFloat(data[1]));
  }
}

void Controllers::Dive::freeze(){
  this->frontMotor->disable();
  this->backMotor->disable();
  // VERBOSE("Dive Enable",1,0);
}

void Controllers::Dive::unfreeze(){
  this->frontMotor->enable();
  this->backMotor->enable();
  // VERBOSE("Dive Enable",1,1);
}
