#include <Components/Chips/ShiftRegister.hpp>

Components::Chips::ShiftRegister::ShiftRegister(Components::Chips::ShiftRegisterConfig config)
  : config(config){
  pinMode(config.shiftClockPin, OUTPUT);
  pinMode(config.storageClockPin, OUTPUT);
  pinMode(config.dataPin, OUTPUT);
}

void Components::Chips::ShiftRegister::shiftInBit(bool state){
  if(state){
    digitalWrite(this->config.dataPin, HIGH); // write on
  }else{
    digitalWrite(this->config.dataPin, LOW); // write off
  }
  digitalWrite(this->config.shiftClockPin, LOW); // reset clock pin
  delay(5);
  digitalWrite(this->config.shiftClockPin, HIGH); // shift bit in
  delay(5);
  digitalWrite(this->config.dataPin, LOW); // reset data pin
  digitalWrite(this->config.shiftClockPin, LOW); // reset clock pin
}

void Components::Chips::ShiftRegister::show(){
  digitalWrite(this->config.storageClockPin, LOW);
  delay(5);
  digitalWrite(this->config.storageClockPin, HIGH);
  delay(5);
  digitalWrite(this->config.storageClockPin, LOW);
}

void Components::Chips::ShiftRegister::pin(int pin, bool state){
  this->pinStates[pin] = state;
  this->blinkEnabled[pin] = false;
}

void Components::Chips::ShiftRegister::blinkPin(int pin){
  this->blinkLastMillis[pin] = millis();
  this->blinkEnabled[pin] = true;
  this->pinStates[pin] = true;
}

void Components::Chips::ShiftRegister::update(){
  for(int i = 0; i < SHIFT_REGISTER_PIN_COUNT; i++){ // change state of pins that need to blink
    if(this->blinkEnabled[i] && (this->blinkLastMillis[i] + this->config.blinkDelay <= millis())){
      this->pinStates[i] = !this->pinStates[i];
      this->blinkLastMillis[i] = millis();
    }
  }

  bool changed = false;
  for(int i = 0; i < SHIFT_REGISTER_PIN_COUNT && !changed; i++){ // check for a change in pin states
    if(this->pinStates[i] != this->oldPinStates[i]){
      changed = true;
    }
  }

  if(changed){
    for(int i = SHIFT_REGISTER_PIN_COUNT - 1; i >= 0; i--){ // update pins backwards
      this->shiftInBit(this->pinStates[i]);
      this->oldPinStates[i] = this->pinStates[i];
    }
    this->show();
  }
}
