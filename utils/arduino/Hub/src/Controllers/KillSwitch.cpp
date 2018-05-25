// #include <Controllers/KillSwitch.hpp>
// #include <Arduino.h>
//
// Controllers::KillSwitch* Controllers::KillSwitch::killSwitch = 0;
//
// byte state = LOW;
// bool running = false;
// bool ledS = false;
// void blink2(){
//   if(!running){
//     running = true;
//     state = digitalRead(3);
//     Timer1.start();
//   }
// }
//
// void blink3(){
//   Timer1.stop();
//   running = false;
//   if(state == digitalRead(3)){
//     ledS = !ledS;
//     if(ledS){
//       digitalWrite(13, HIGH);
//     }else{
//       digitalWrite(13, LOW);
//     }
//   }
// }
//
// Controllers::KillSwitch::KillSwitch(int pin, int32_t handler )
//   :handler(handler){
//   //stopped = false;
//
//   pinMode(13, OUTPUT);
//   *portConfigRegister(3) = PORT_PCR_MUX(1) | PORT_PCR_PE; // pull input pin low
//
//   Timer1.initialize(1000000); // will ignore interupts for this ammount of us, and then check if the state is the same as the expected changed state
//   Timer1.attachInterrupt(blink3);
//   Timer1.stop();
//   ledS = digitalRead(3);
//   digitalWrite(13, ledS);
//
//   //Timer1.restart();
//
//   //pinMode(pin, INPUT_PULLUP);
//   //pinMode(pin, INPUT);
//   //digitalWrite(13, HIGH);
//
//   //Controllers::KillSwitch::killSwitch = this;
//   attachInterrupt(digitalPinToInterrupt(3), blink2/*Controllers::KillSwitch::interrupt*/, CHANGE);
// }
//
// void Controllers::KillSwitch::execute(Emitter* emitter, int32_t* data, int32_t length){
//   this->emitter->kill();
// }
//
// void Controllers::KillSwitch::use(Emitter* emitter){
//   this->emitter = emitter;
//   int32_t init[1] = {123};
//   emitter->emit(42, init, 1);
//   while(1){
//     int32_t data[1] = {digitalRead(3)};
//     emitter->emit(42, data, 1);
//     delay(100);
//   }
// }
//
// void Controllers::KillSwitch::interrupt(){
//   int32_t reportData[1] = {123};
//   killSwitch->emitter->emit(killSwitch->handler, reportData, 1);
//   Controllers::KillSwitch* killSwitch = Controllers::KillSwitch::killSwitch;
//   int32_t data[1] = {  };
//
//   if( !killSwitch->stopped ) { //Send kill command
//     killSwitch->stopped = true; //controllers now stopped
//     killSwitch->emitter->kill();
//     data[0] = 0;
//     killSwitch->emitter->emit(killSwitch->handler, data, 1);
//     killSwitch->numInterrupts = 0;
//
//   } else {
//
//     if( killSwitch->numInterrupts == 0 || millis() <= (killSwitch->lastInterrupt + killSwitch->maxSwitchTime) ) {
//       killSwitch->numInterrupts+=1;
//
//       if(killSwitch->numInterrupts >= (killSwitch->numSwitches * 2 - 1)) {
//         delay(killSwitch->waitTime);
//         killSwitch->stopped = false;
//         killSwitch->emitter->restart();
//         data[0] = 1;
//         killSwitch->emitter->emit(killSwitch->handler, data, 1);
//         killSwitch->numInterrupts = 0;
//
//       }
//     } else if ( killSwitch->numInterrupts > 0 && millis() > (killSwitch->lastInterrupt + killSwitch->maxSwitchTime) ) {
//       killSwitch->numInterrupts = (killSwitch->numInterrupts + 1)% 2; //If timedout ensure reset to appropriate number of interrupts
//     }
//     killSwitch->lastInterrupt = millis();
//   }
// }
