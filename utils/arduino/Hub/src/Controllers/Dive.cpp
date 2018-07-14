#include <Controllers/Dive.hpp>

#define BLUE_ROBOTICS_ESC_PWM_REVERSE 1100
#define BLUE_ROBOTICS_ESC_PWM_FORWARD 1900

Controllers::Dive::Dive(int frontPin, int backPin, bool protectMotors)
  : frontPin(frontPin),
    backPin(backPin),
    protectMotors(protectMotors),
    stopped(true) { 
  this->unfreeze();      
}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 1 && !stopped){
    // this->front.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    // this->back.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    this->front.writeMicroseconds(map(this->protectMotors ? constrain(data[0], 10, 170) : data[0], 0, 180, BLUE_ROBOTICS_ESC_PWM_REVERSE, BLUE_ROBOTICS_ESC_PWM_FORWARD));
    this->back.writeMicroseconds(map(this->protectMotors ? constrain(data[0], 10, 170) : data[0], 0, 180, BLUE_ROBOTICS_ESC_PWM_REVERSE, BLUE_ROBOTICS_ESC_PWM_FORWARD));
  }
}

void Controllers::Dive::freeze(){
  stopped = true;
  this->front.writeMicroseconds(1500);
  this->back.writeMicroseconds(1500);
  this->front.detach();
  this->back.detach();
}

void Controllers::Dive::unfreeze(){
  stopped = false;
  digitalWrite(13, HIGH);
  this->front.attach(this->frontPin);
  this->back.attach(this->backPin);
  this->front.writeMicroseconds(1500);
  this->back.writeMicroseconds(1500);
  delay(1000);
  digitalWrite(13, LOW);
}
