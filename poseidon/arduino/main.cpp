#include <Hub.hpp>
//#include <Controllers/DC_MotorDriver.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Controllers/Dive.hpp>
#include <PinMap.hpp>
#include <Arduino.h>

Hub* hub;
Controller** controllers;

void setup()
{
  controllers = new Controller*[3];
  controllers[0] = new Controllers::Echo(ECHO_RETURN);
  controllers[1] = new Controllers::KillSwitch(KILL_PIN, ECHO_RETURN, MILLI_SECONDS(200));
  controllers[2] = new Controllers::Dive(4, 5);
  hub = new Hub(controllers, 3);
  ((Controllers::KillSwitch*)controllers[1])->use(hub, hub);
  pinMode(13, OUTPUT);
}

int count = 0;
bool state = true;

void loop() {
  hub->poll();
  // if(++count > 500){
  //   count = 0;
  //   state = !state;
  //   digitalWrite(13, state);
  // }
  delay(1);
}
