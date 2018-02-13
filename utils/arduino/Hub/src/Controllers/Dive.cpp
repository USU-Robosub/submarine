#include <Controllers/Dive.hpp>

Controllers::Dive::Dive(int frontPin, int backPin, bool protectMotors)
  : protectMotors(protectMotors) {
  front.attach(frontPin);
  back.attach(backPin);
  front.write(90);
  back.write(90);
}

void Controllers::Dive::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length == 1){
    front.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
    back.write(this->protectMotors ? constrain(data[0], 10, 170) : data[0]);
  }
}
