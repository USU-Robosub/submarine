#include <Servo.mock.hpp>

bool Servo::attached[] = {false};
int Servo::speed[NUM_SERVOS] = {90};

void Servo::reset(){
  for(int i = 0; i < NUM_SERVOS; ++i){
    Servo::attached[i] = false;
    Servo::speed[i] = 90;
  }
}
