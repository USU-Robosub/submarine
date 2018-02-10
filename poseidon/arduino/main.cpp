#include <Hub.hpp>
#include <Controllers/Tank.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/Dive.hpp>
#include <PinMap.hpp>

#ifndef UNIT_TESTS
  #include <Arduino.h>

  #ifndef TEENSY
    #include <Servo.h>
  #else // teensy boards use a different library for servos
    #include <PWMServo.h>
    #define Servo PWMServo
  #endif

#else
  #include <Servo.mock.hpp>
#endif

// class Blink : public Controller{
// public:
//   Blink() : time(0), delay(200) {
//     pinMode(LED_BUILTIN, OUTPUT);
//   }
//   void update() {
//     this->time++;
//     if(this->time < this->delay){
//       digitalWrite(LED_BUILTIN, HIGH);
//     }else if(this->time < this->delay * 2){
//       digitalWrite(LED_BUILTIN, LOW);
//     }else{
//       time = 0;
//     }
//   }
//   void execute(Emitter* hub, long* data, long length){
//     delay = data[0];
//     hub->emit(10, data, 1);
//   }
// private:
//   unsigned int time;
//   unsigned int delay;
// };

#define ECHO_RETURN 42

Hub* hub;
Controller** controllers;

void setup()
{
  controllers = new Controller*[3];
  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::Tank(LEFT_THRUSTER, RIGHT_THRUSTER);
  controllers[2] = new Controllers::Dive(FRONT_THRUSTER, BACK_THRUSTER);
  hub = new Hub(controllers, 3);
}

void loop() {
  hub->poll();
  delay(1);
}
