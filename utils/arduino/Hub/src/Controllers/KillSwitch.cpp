#include <Controllers/KillSwitch.hpp>
#include <Arduino.h>

Controllers::KillSwitch* Controllers::KillSwitch::killSwitch = 0;

Controllers::KillSwitch::KillSwitch(int pin, int32_t handler )
  :handler(handler){
  stopped = false;
  pinMode(pin, INPUT_PULLUP);
  Controllers::KillSwitch::killSwitch = this;
  attachInterrupt(digitalPinToInterrupt(pin), Controllers::KillSwitch::interrupt, CHANGE);
}

void Controllers::KillSwitch::execute(Emitter* emitter, int32_t* data, int32_t length){
  this->emitter->kill();
}

void Controllers::KillSwitch::use(Emitter* emitter){
  this->emitter = emitter;
}

void Controllers::KillSwitch::interrupt(){
  Controllers::KillSwitch* killSwitch = Controllers::KillSwitch::killSwitch;
  int32_t data[1] = {  };
  
  if( !killSwitch->stopped ) { //Send kill command
    killSwitch->stopped = true; //controllers now stopped
    killSwitch->emitter->kill();
    data[0] = 0;
    killSwitch->emitter->emit(killSwitch->handler, data, 1);
    killSwitch->numInterrupts = 0;
    
  } else {
    
    if( killSwitch->numInterrupts == 0 || millis() <= (killSwitch->lastInterrupt + killSwitch->maxSwitchTime) ) {
      killSwitch->numInterrupts+=1;
      
      if(killSwitch->numInterrupts >= (killSwitch->numSwitches * 2 - 1)) {
        delay(killSwitch->waitTime);
        killSwitch->stopped = false;
        killSwitch->emitter->restart();
        data[0] = 1;
        killSwitch->emitter->emit(killSwitch->handler, data, 1);
        killSwitch->numInterrupts = 0;
        
      }
    } else if ( killSwitch->numInterrupts > 0 && millis() > (killSwitch->lastInterrupt + killSwitch->maxSwitchTime) ) {
      killSwitch->numInterrupts = (killSwitch->numInterrupts + 1)% 2; //If timedout ensure reset to appropriate number of interrupts
    }
    killSwitch->lastInterrupt = millis();
  }
}
