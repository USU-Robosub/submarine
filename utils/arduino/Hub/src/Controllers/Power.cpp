#include <Controllers/Power.hpp>
#include <Log.hpp>
#include <Hub.hpp>

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
      delay(1000);
      static_cast<Hub*>(hub)->unfreeze();
    }else if(data[0] != 1){
      static_cast<Hub*>(hub)->freeze();
      delay(1000);
      digitalWrite(this->pin, HIGH); // disable
    }
  }
}

void Controllers::Power::freeze(){
  digitalWrite(this->pin, HIGH); // disable
}

void Controllers::Power::unfreeze(){

}
