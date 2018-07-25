#include <Controllers/KillSwitch.hpp>
#include <Arduino.h>

Controllers::KillSwitch* Controllers::KillSwitch::killSwitch = nullptr;
bool Controllers::KillSwitch::checkingDebounce = false;
bool Controllers::KillSwitch::startSate = false;

Controllers::KillSwitch::KillSwitch(int pin, int32_t handler, int debounceDelay)
  : handler(handler),
    pin(pin),
    freezable(nullptr),
    emitter(nullptr){

  //#if !defined(TEENSY) || UNIT_TESTS \\ TODO remove this if it works without
  pinMode(pin, INPUT);
  //#endif
  #if defined(TEENSY) || UNIT_TESTS
    *portConfigRegister(pin) = PORT_PCR_MUX(1) | PORT_PCR_PE; // pull input pin low on teensy
  #endif

  MsTimer2::set(debounceDelay, Controllers::KillSwitch::debounce); // will ignore interupts for this ammount of us, and then check if the state is the same as the expected changed state

  Controllers::KillSwitch::killSwitch = this;
  attachInterrupt(digitalPinToInterrupt(pin), Controllers::KillSwitch::interrupt, CHANGE);
}

void Controllers::KillSwitch::execute(Emitter* emitter, int32_t* data, int32_t length){
  (void) emitter;
  (void) data;
  (void) length;
  // do nothing
}

void Controllers::KillSwitch::use(Freezable* freezable, Emitter* emitter){
  this->emitter = emitter;
  this->freezable = freezable;
  this->freezable->freeze();
}

void Controllers::KillSwitch::interrupt(){
  if(!Controllers::KillSwitch::checkingDebounce){
    Controllers::KillSwitch::checkingDebounce = true;
    Controllers::KillSwitch::startSate = digitalRead(killSwitch->pin);
    MsTimer2::start();
  }
}

void Controllers::KillSwitch::debounce(){
  MsTimer2::stop();
  Controllers::KillSwitch::checkingDebounce = false;
  if(Controllers::KillSwitch::startSate == digitalRead(killSwitch->pin)){
    if(Controllers::KillSwitch::startSate == HIGH){
      killSwitch->freezable->unfreeze();
      int32_t data[1] = {1};
      if(killSwitch->emitter != nullptr){
        killSwitch->emitter->emit(killSwitch->handler, data, 1);
      }
    }else{
      killSwitch->freezable->freeze();
      int32_t data[1] = {0};
      if(killSwitch->emitter != nullptr){
        killSwitch->emitter->emit(killSwitch->handler, data, 1);
      }
    }
  }
}
