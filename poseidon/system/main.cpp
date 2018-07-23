#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/Serial/PortFinder.hpp>
#include <Comm/TCP/FullStack.hpp>
#include <Vision/Livestream.hpp>
#include <Comm/tools.hpp>
#include <Subsystem/Dive.hpp>
#include <Subsystem/Tank.hpp>

#include "settings.hpp"

#include <thread>
#include <chrono>
#include <string>

void createHubs();
void createSubsystems();
void runEventLoop();

// hubs
Comm::Serial::FullStack* arduino;
Comm::TCP::FullStack* agent;

// subsystems
Subsystem::Dive* dive;
Subsystem::Tank* tank;

// event loop
bool stopApp = false;

int main(){
  createHubs();
  createSubsystems();
  runEventLoop();
  return 0;
}

void createHubs(){
  try{
    std::cout << Comm::Serial::PortFinder::findByPath(ARDUINO_PORT) << std::endl;
    arduino = new Comm::Serial::FullStack("/dev/serial/by-path/" + Comm::Serial::PortFinder::findByPath(ARDUINO_PORT), ARDUINO_BAUD);
    std::cout << "test" << std::endl;
  }catch(std::runtime_error e){
    arduino = new Comm::Serial::FullStack(ARDUINO_PORT_EMULATED, ARDUINO_BAUD);
  }
  arduino->restartArduino();
  agent = new Comm::TCP::FullStack(AGENT_PORT, AGENT_DELIMITER);
  std::cout << "Created network" << std::endl;
}

void createSubsystems(){
  //arduino.hub()->on(1,[&agent](std::vector<int> message){
  //  bool enable = message.size()>0&&message.at(0)==1;
  //  agent.hub()->emit("killswitch", std::vector<std::string>{(enable?"1":"0")});
  //});

  arduino->hub()->on(ECHO_PORT_NUM, [](std::vector<int> message){
    std::cout << "Echo from arduino: " << message[0] << std::endl;
  });

  dive = new Subsystem::Dive(arduino->hub(), DIVE_PORT_NUM, agent->hub(), DIVE_PORT_NAME);
  tank = new Subsystem::Tank(arduino->hub(), TANK_PORT_NUM, agent->hub(), TANK_PORT_NAME);
}

void runEventLoop(){
  while(!stopApp){
    arduino->hub()->poll();
    agent->hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY));
  }
}
