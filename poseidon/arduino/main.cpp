#include <Hub.hpp>
#include <settings.hpp>
#include <Arduino.h>

#include <Components/Motors/BlueRoboticsR1Esc.hpp>
#include <Components/Chips/ShiftRegister.hpp>

#include <Controllers/Empty.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Controllers/Dive.hpp>
#include <Controllers/Tank.hpp>

typedef Components::Motors::BlueRoboticsR1Esc Motor;
typedef Components::Chips::ShiftRegister ShiftRegister;

struct Thrusters{
  Motor* front;
  Motor* back;
  Motor* left;
  Motor* right;
};

#define CONTROLLER_COUNT 4

Hub* hub;
Controller** controllers;
Thrusters thrusters;
ShiftRegister* shiftRegister;


void createComponents(){
  thrusters.front = new Motor({.pin=FRONT_MOTOR_PIN, .trim=MOTOR_DEFAULT_TRIM}),
  thrusters.back = new Motor({.pin=BACK_MOTOR_PIN, .trim=MOTOR_DEFAULT_TRIM}),
  thrusters.left = new Motor({.pin=LEFT_MOTOR_PIN, .trim=MOTOR_DEFAULT_TRIM}),
  thrusters.right = new Motor({.pin=RIGHT_MOTOR_PIN, .trim=MOTOR_DEFAULT_TRIM});
}

void createControllers(){
  controllers = new Controller*[CONTROLLER_COUNT];

  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::Empty();
  //controllers[1] = new Controllers::KillSwitch(KILL_PIN, ECHO_RETURN, MILLI_SECONDS(200));

  controllers[2] = new Controllers::Dive(thrusters.front, thrusters.back);
  controllers[3] = new Controllers::Tank(thrusters.left, thrusters.right);
}

void setupControllers(){
  //static_cast<Controllers::KillSwitch*>(controllers[1])->use(hub, hub);
  static_cast<Controllers::Dive*>(controllers[2])->unfreeze();
  static_cast<Controllers::Tank*>(controllers[3])->unfreeze();
}

void connectToSerial(){
  hub = new Hub(controllers, CONTROLLER_COUNT);
}

void pollSerialData(){
  hub->poll();
}

void setup()
{
  shiftRegister = new ShiftRegister({.shiftClockPin=10, .storageClockPin=11, .dataPin=12, .blinkDelay=500});
  // shiftRegister->blinkPin(0);
  // shiftRegister->blinkPin(2);
  // shiftRegister->blinkPin(5);
  // shiftRegister->blinkPin(7);
  createComponents();
  createControllers();
  connectToSerial();
  setupControllers();
}

//bool state = false;
int index = 0;
unsigned long lastMillis = 0;

void loop() {
  if(lastMillis + 75 < millis()){
    lastMillis = millis();

    // spin
    // shiftRegister->pin(index >= 4 ? 11 - index : index, LOW);
    // index = (index + 1) % 8;
    // shiftRegister->pin(index >= 4 ? 11 - index : index, HIGH);

    // bounce
    // shiftRegister->pin(abs(-index + 3) + 4, LOW);
    // index = (index + 1) % 6;
    // shiftRegister->pin(abs(-index + 3) + 4, HIGH);

    // progress
    // if(index <= 3){
    //   shiftRegister->pin(index + 4, LOW);
    // }
    // index = (index + 1) % 8;
    // if(index + 1 <= 3){
    //   shiftRegister->pin(index + 4, HIGH);
    //   shiftRegister->pin(index + 5, HIGH);
    // }

    // fast bounce
    // shiftRegister->pin(abs(-index + 2) + 4, LOW);
    // shiftRegister->pin(abs(-index + 2) + 5, LOW);
    // index = (index + 1) % 4;
    // shiftRegister->pin(abs(-index + 2) + 4, HIGH);
    // shiftRegister->pin(abs(-index + 2) + 5, HIGH);

    // large bounce
    // shiftRegister->pin(0, LOW);
    // shiftRegister->pin(1 + 4, LOW);
    // shiftRegister->pin(2, LOW);
    // shiftRegister->pin(3 + 4, LOW);
    // index = (index + 1) % 18;
    // int pos = abs(-index + 9) - 3;
    // if(abs(pos) < 2){
    //   shiftRegister->pin(0, HIGH);
    // }
    // if(abs(pos - 1) < 2){
    //   shiftRegister->pin(1 + 4, HIGH);
    // }
    // if(abs(pos - 2) < 2){
    //   shiftRegister->pin(2, HIGH);
    // }
    // if(abs(pos - 3) < 2){
    //   shiftRegister->pin(3 + 4, HIGH);
    // }

    shiftRegister->pin(0, LOW);
    shiftRegister->pin(1 + 4, LOW);
    shiftRegister->pin(2, LOW);
    shiftRegister->pin(3 + 4, LOW);

    shiftRegister->pin(0 + 4, LOW);
    shiftRegister->pin(1, LOW);
    shiftRegister->pin(2 + 4, LOW);
    shiftRegister->pin(3, LOW);

    index = (index + 1) % 26;
    int pos = abs(-index + 13) - 3;

    if(abs(pos) < 2){
      shiftRegister->pin(0, HIGH);
    }
    if(abs(pos - 1) < 2){
      shiftRegister->pin(1, HIGH);
    }
    if(abs(pos - 2) < 2){
      shiftRegister->pin(2, HIGH);
    }
    if(abs(pos - 3) < 2){
      shiftRegister->pin(3, HIGH);
    }

    if(abs(pos - 4) < 2){
      shiftRegister->pin(7, HIGH);
    }
    if(abs(pos - 5) < 2){
      shiftRegister->pin(6, HIGH);
    }
    if(abs(pos - 6) < 2){
      shiftRegister->pin(5, HIGH);
    }
    if(abs(pos - 7) < 2){
      shiftRegister->pin(4, HIGH);
    }
  }
  // state = !state;
  // shiftRegister->pin(0, state);
  shiftRegister->update();
  pollSerialData();
  delay(LOOP_DELAY);
}
