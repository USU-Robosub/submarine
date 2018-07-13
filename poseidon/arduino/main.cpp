#include <Hub.hpp>
#include <PinMap.hpp>
#include <Arduino.h>

#include <Components/Motors/BlueRoboticsR1Esc.hpp>

#include <Controllers/Empty.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Controllers/Dive.hpp>
#include <Controllers/Tank.hpp>

typedef Components::Motors::BlueRoboticsR1Esc Motor;

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
}

void connectToSerial(){
  hub = new Hub(controllers, CONTROLLER_COUNT);
}

void pollSerialData(){
  hub->poll();
}

void setup()
{
  createComponents();
  createControllers();
  connectToSerial();
  setupControllers();
}

void loop() {
  pollSerialData();
  delay(LOOP_DELAY);
}
