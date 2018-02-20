#include <Controllers/DC_MotorDriver.hpp>

Controllers::DC_MotorDriver::DC_MotorDriver(int pinSpeedA, int pinForwardA, int pinBackwardA, int pinSpeedB, int pinForwardB, int pinBackwardB, bool protectMotors)
  : pinSpeedA(pinSpeedA),
    pinForwardA(pinForwardA),
    pinBackwardA(pinBackwardA),
    pinSpeedB(pinSpeedB),
    pinForwardB(pinForwardB),
    pinBackwardB(pinBackwardB),
    protectMotors(protectMotors) {

  pinMode(this->pinSpeedA,OUTPUT);
  pinMode(this->pinForwardA,OUTPUT);
  pinMode(this->pinBackwardA,OUTPUT);

  pinMode(this->pinSpeedB,OUTPUT);
  pinMode(this->pinForwardB,OUTPUT);
  pinMode(this->pinBackwardB,OUTPUT);
}

void Controllers::DC_MotorDriver::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2){
    int mixLeft =  (data[0] + (data[1] - 90) - 90) * 2;
    int mixRight =  (data[0] - (data[1] - 90) - 90) * 2;

    int speedA = this->protectMotors ? constrain(mixLeft, -180, 180) : mixLeft;
    int speedB = this->protectMotors ? constrain(mixRight, -180, 180) : mixRight;

    digitalWrite(this->pinForwardA, LOW ); digitalWrite(this->pinBackwardA, LOW);
    digitalWrite(this->pinForwardB, LOW ); digitalWrite(this->pinBackwardB, LOW);

    analogWrite(this->pinSpeedA, abs(speedA));
    analogWrite(this->pinSpeedB, abs(speedB));

    if(speedA > 0)      { digitalWrite(this->pinBackwardA,LOW ); digitalWrite(this->pinForwardA,HIGH); }
    else if(speedA < 0) { digitalWrite(this->pinBackwardA,HIGH); digitalWrite(this->pinForwardA,LOW ); }
    else                { digitalWrite(this->pinBackwardA,LOW ); digitalWrite(this->pinForwardA,LOW ); }

    if(speedB > 0)      { digitalWrite(this->pinBackwardB,LOW ); digitalWrite(this->pinForwardB,HIGH); }
    else if(speedB < 0) { digitalWrite(this->pinBackwardB,HIGH); digitalWrite(this->pinForwardB,LOW ); }
    else                { digitalWrite(this->pinBackwardB,LOW ); digitalWrite(this->pinForwardB,LOW ); }
  }
}

void Controllers::DC_MotorDriver::stop(){

}
