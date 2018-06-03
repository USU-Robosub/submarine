#include <Hub.hpp>
#include <Controllers/DC_MotorDriver.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
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

Hub* hub;
Controller** controllers;

void setup()
{
  controllers = new Controller*[3];
  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::KillSwitch(KILL_PIN, 42);
  //controllers[2] = new Controllers::Dive(TEST_MOTOR_PIN, TEST_MOTOR_PIN);
  //hub = new Hub(controllers, 3);
}

void loop() {
  //hub->poll();
  //Serial.print("2");
  Serial.print(digitalRead(3));
  delay(1);
}


// #include <Arduino.h>

// Servo servo;

// void setup(){
//   //pinMode(3, INPUT);
//   *portConfigRegister(3) = PORT_PCR_MUX(1) | PORT_PCR_PE; // pull input pin low
  
//   // Serial.begin(9600);
  
//   servo.attach(4);
// }

// void loop() {
//   //Serial.print("hello world");
//   //Serial.print(digitalRead(3));
//   for(int i = 0; i < 180 && digitalRead(3); i++){
//     servo.write(i);
//     delay(10);
//   }
//   for(int i = 180; i > 0 && digitalRead(3); i--){
//     servo.write(i);
//     delay(10);
//   }
// }

// const byte ledPin = 13;
// const byte interruptPin = 3;
// volatile bool state = true;

// void blink() {
//   state = !state;
//   if(state){
//     digitalWrite(ledPin, HIGH);
//   }else{
//     digitalWrite(ledPin, LOW);
//   }
// }

// void setup() {
//   pinMode(ledPin, OUTPUT);
//   //pinMode(interruptPin, INPUT_PULLUP);
//   *portConfigRegister(3) = PORT_PCR_MUX(1) | PORT_PCR_PE; // pull input pin low
//   attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
// }

// void loop() {
//   // if(state){
//   //   digitalWrite(ledPin, HIGH);
//   //   delay(500);
//   //   digitalWrite(ledPin, LOW);
//   //   delay(500);
//   // }
// }

