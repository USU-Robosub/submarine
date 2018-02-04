#include <Hub.hpp>

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

#define numServos 1
extern "C" const int servoPorts[] = { 13 };

class Error : public Controller{
public:

  void execute(Emitter* hub, long* data, long length){
    // error reading from serial
  }
};

class Motor : public Controller{
public:
  Motor() {
    for(int i = 0;i < numServos;i++) {
      _servo[i].attach(servoPorts[i]);
      _servo[i].write(90);
    }
  }

  void execute(Emitter* hub, long* data, long length){
    if(length<2||data[0]>numServos||data[0]<0||data[1]>180||data[1]<0)
      return;
    if(data[0]>0) {
      _servo[data[1]-1].write(data[1]);
    } else {
      for(int i = 0;i < numServos;i++) {
        _servo[i].write(data[1]);
      }
    }
  }
private:
  Servo _servo[numServos];
};

Hub* _hub;
Controller** _controllers;

void setup()
{
  _controllers = new Controller*[2];
  _controllers[0] = new Error;
  _controllers[1] = new Motor;
  _hub = new Hub(_controllers, 2);
}

void loop() {
  _hub->poll();
}
