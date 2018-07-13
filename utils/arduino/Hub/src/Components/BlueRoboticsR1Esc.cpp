#include <Components/Motors/BlueRoboticsR1Esc.hpp>

Components::Motors::BlueRoboticsR1Esc::BlueRoboticsR1Esc(Components::Motors::BlueRoboticsR1EscConfig config)
  : pin(config.pin),
    maxReversePower(map(config.trim.maxReverse, MOTOR_REVERSE, MOTOR_STOP, BR_R1_PWM_MOTOR_REVERSE, BR_R1_PWM_MOTOR_STOP)),
    minReversePower(map(config.trim.minReverse, MOTOR_REVERSE, MOTOR_STOP, BR_R1_PWM_MOTOR_REVERSE, BR_R1_PWM_MOTOR_STOP)),
    stopPower(config.trim.stop >= MOTOR_STOP ?
      map(config.trim.stop, MOTOR_REVERSE, MOTOR_STOP, BR_R1_PWM_MOTOR_REVERSE, BR_R1_PWM_MOTOR_STOP) :
      map(config.trim.stop, MOTOR_STOP, MOTOR_FORWARD, BR_R1_PWM_MOTOR_STOP, BR_R1_PWM_MOTOR_FORWARD)
    ),
    minForwardPower(map(config.trim.minForward, MOTOR_STOP, MOTOR_FORWARD, BR_R1_PWM_MOTOR_STOP, BR_R1_PWM_MOTOR_FORWARD)),
    maxForwardPower(map(config.trim.maxForward, MOTOR_STOP, MOTOR_FORWARD, BR_R1_PWM_MOTOR_STOP, BR_R1_PWM_MOTOR_FORWARD)),
    isEnabled(false){

}

void Components::Motors::BlueRoboticsR1Esc::power(float amount){
  if(this->isEnabled){
    if(amount == 0){
      this->stop();
    }else if(amount > 0){
      this->forward(amount);
    }else{
      this->reverse(amount);
    }
  }
}

void Components::Motors::BlueRoboticsR1Esc::enable(){
  this->isEnabled = true;
  this->servo.attach(this->pin);
  this->stop();
}

void Components::Motors::BlueRoboticsR1Esc::disable(){
  this->isEnabled = false;
  this->servo.detach();
  this->stop();
}

void Components::Motors::BlueRoboticsR1Esc::stop(){
  this->servo.writeMicroseconds(this->stopPower);
}

void Components::Motors::BlueRoboticsR1Esc::forward(float amount){
  float constrainedAmount = constrain(amount, MOTOR_STOP, MOTOR_FORWARD);
  this->servo.writeMicroseconds(
    map(constrainedAmount, MOTOR_STOP, MOTOR_FORWARD, this->minForwardPower, this->maxForwardPower)
  );
}

void Components::Motors::BlueRoboticsR1Esc::reverse(float amount){
  float constrainedAmount = constrain(amount, MOTOR_REVERSE, MOTOR_STOP);
  this->servo.writeMicroseconds(
    map(constrainedAmount, MOTOR_REVERSE, MOTOR_STOP, this->maxReversePower, this->minReversePower)
  );
}
