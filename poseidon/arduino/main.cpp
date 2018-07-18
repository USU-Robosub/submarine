#include <Hub.hpp>
#include <PinMap.hpp>
#include <Arduino.h>

#include <Components/Motors/BlueRoboticsR1Esc.hpp>
#include <Components/Chips/ShiftRegister.hpp>
#include <Components/Sensors/HMC5883L.hpp>

#include <Controllers/Empty.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Controllers/Dive.hpp>
#include <Controllers/Tank.hpp>

typedef Components::Motors::BlueRoboticsR1Esc Motor;
typedef Components::Chips::ShiftRegister ShiftRegister;
typedef Components::Sensors::HMC5883L Magnetometer;

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
Magnetometer* magnetometer;


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
  magnetometer = new Magnetometer();
  shiftRegister = new ShiftRegister({.shiftClockPin=10, .storageClockPin=11, .dataPin=12, .blinkDelay=500});
  // shiftRegister->blinkPin(0);
  // shiftRegister->blinkPin(2);
  // shiftRegister->blinkPin(5);
  // shiftRegister->blinkPin(7);
  createComponents();
  createControllers();
  connectToSerial();
  setupControllers();
  
  pinMode(13, OUTPUT);
}

bool state = true;
long lastMillis = 0;

void loop() {
  magnetometer->update();
  int32_t data[3] = {magnetometer->x(), magnetometer->y(), magnetometer->z()};
  hub->emit(4, data, 3);
  
  if(lastMillis + 200 < millis()){
    lastMillis = millis();
    state = !state;
    digitalWrite(13, state);
  }
  
  shiftRegister->update();
  pollSerialData();
  Serial.flush();
  delay(LOOP_DELAY);
}
