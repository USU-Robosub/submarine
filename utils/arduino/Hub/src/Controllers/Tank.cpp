#include <Controllers/Tank.hpp>

Controllers::Tank::Tank(int leftPin, int rightPin, bool protectMotors)
  : protectMotors(protectMotors) {
  left.attach(leftPin);
  right.attach(rightPin);
}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2){
    int mixLeft =  data[0] + (data[1] - 90);
    int mixRight =  data[0] - (data[1] - 90);
    left.write(this->protectMotors ? constrain(mixLeft, 10, 170) : mixLeft);
    right.write(this->protectMotors ? constrain(mixRight, 10, 170) : mixRight);
  }
}
