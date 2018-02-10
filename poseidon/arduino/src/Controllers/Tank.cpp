#include <Controllers/Tank.hpp>

Controllers::Tank::Tank(int leftPin, int rightPin, bool protectMotors)
  : protectMotors(protectMotors) {
  left.attach(leftPin);
  right.attach(rightPin);
}

void Controllers::Tank::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 2){
    left.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    right.write(this->protectMotors ? constrain(data[1], 10, 170) : data[1]);
  }
}
