#include <Controllers/Power.hpp>
#include <Log.hpp>

Controllers::Power::Power(unsigned int pin)
  : pin(pin){
  pinMode(pin, OUTPUT);
  digitalWrite(this->pin, HIGH); // disable
}

void Controllers::Power::execute(Emitter* hub, int32_t* data, int32_t length){
  if(length >= 1){
    INFO("Arduino Power", data[0]);
    if(data[0] == 1){
      digitalWrite(this->pin, LOW); // enable
    }else if(data[0] != 1){
      digitalWrite(this->pin, HIGH); // disable
    }
  }
}

void Controllers::Power::freeze(){
  digitalWrite(this->pin, HIGH); // disable
}

void Controllers::Power::unfreeze(){
  
}
