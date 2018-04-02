#include <Controllers/KillSwitch.hpp>
#include <Arduino.h>

Controllers::KillSwitch::KillSwitch(int pin, int32_t handler, Controller** controllers){
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), Controllers::KillSwitch::interrupt, CHANGE);
}

void Controllers::KillSwitch::execute(Emitter* emitter, int32_t* data, int32_t length){
  // ignore all
}

void Controllers::KillSwitch::use(Emitter* emitter){

}

void Controllers::KillSwitch::interrupt(){
  int data[1] = {  };
  this->emitter->emit(this->handler, data, 1);
}
