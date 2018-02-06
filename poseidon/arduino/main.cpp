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

class Blink : public Controller{
public:
  Blink() : time(0), delay(200) {
    pinMode(LED_BUILTIN, OUTPUT);
  }
  void update() {
    this->time++;
    if(this->time < this->delay){
      digitalWrite(LED_BUILTIN, HIGH);
    }else if(this->time < this->delay * 2){
      digitalWrite(LED_BUILTIN, LOW);
    }else{
      time = 0;
    }
  }
  void execute(Emitter* hub, long* data, long length){
    delay = data[0];
    hub->emit(10, data, 1);
  }
private:
  unsigned int time;
  unsigned int delay;
};

class Echo : public Controller{
public:
  Echo(long name) : name(name) {}
  void execute(Emitter* hub, long* data, long length){
    hub->emit(this->name, data, length);
  }
private:
  long name;
};

Hub* _hub;
Controller** _controllers;
Blink* blink;

void setup()
{
  _controllers = new Controller*[3];
  _controllers[0] = new Error;
  _controllers[1] = new Echo(42);
  _controllers[2] = blink = new Blink;
  //_controllers[1] = new Motor;
  _hub = new Hub(_controllers, 3);
}

//long count = 0;

long read(){
  return Serial.read();
}

long readOneLong()
{
  return (read()) |
         (read() << 8) |
         (read() << 16) |
         (read() << 24);
}

void writeOneLong(long value)
{
  Serial.write((char*)&value, 4);
}


void loop() {
  //++count;
  //long data[] = { count };
  //_hub->emit(10, data, 1);
  //_hub->poll();
  //blink->update();
  //delay(10);
  if(Serial.available() >= 4*4){
    long empty = readOneLong();
    long name = readOneLong();
    long length = readOneLong();
    long data = readOneLong();
    writeOneLong(0);
    writeOneLong(42);
    writeOneLong(1);
    // writeOneLong(empty);
    // writeOneLong(name);
    // writeOneLong(length);
    writeOneLong(data);
  }
  delay(1);
}
