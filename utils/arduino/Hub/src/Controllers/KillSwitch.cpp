#include <Controllers/KillSwitch.hpp>
#include <Arduino.h>

Controllers::KillSwitch::KillSwitch(int pin, int32_t handler ){
  stopped = false;
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), Controllers::KillSwitch::interrupt, CHANGE);
}

void Controllers::KillSwitch::execute(Emitter* emitter, int32_t* data, int32_t length){
  this->emitter->kill();
}

void Controllers::KillSwitch::use(Emitter* emitter){
  this->emitter = emitter;
}

void Controllers::KillSwitch::interrupt(){
  int data[1] = {  };
  if( !stopped ) { //Send kill command
    stopped = true; //controllers now stopped
    this->emitter->kill();
    data[0] = 0;
    this->emitter->emit(this->handler, data, 1);
    numInterrupts = 0;
  } else {
    if( numInterrupts == 0 || millis() <= (lastInterrupt + maxSwitchTime) ) {
      numInterrupts+=1;
      if(numInterrupts >= (numSwitches * 2 - 1) {
        delay(waitTime);
        stopped = false;
        this->emitter->restart();
        data[0] = 1;
        this->emitter->emit(this->handler, data, 1);
        numInterrupts = 0;
      }
    } else if ( numInterrupts > 0 && millis() > (lastInterrupt + maxSwitchTime) ) {
      numInterrupts = (numInterrupts + 1)% 2; //If timedout ensure reset to appropriate number of interrupts
    }
    lastInterrupt = millis();
  }
}
