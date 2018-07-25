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

Vision::Livestream* vision3;

// event loop
bool stopApp = false;

int main(){
  createHubs();
  createSubsystems();
  runEventLoop();
  return 0;
}

void createHubs(){
  std::cout << "Connecting to Vision" << std::endl;
  try{
    vision3 = new Vision::Livestream("1-1.3.3",8080,100);
  }catch(...){
    std::cerr << "Could not connect to camera." << std::endl;
  }
  std::cout << "Connecting to arduino" << std::endl;
  try{
    std::cout << Comm::Serial::PortFinder::findByPath(ARDUINO_PORT) << std::endl;
    arduino = new Comm::Serial::FullStack("/dev/serial/by-path/" + Comm::Serial::PortFinder::findByPath(ARDUINO_PORT), ARDUINO_BAUD);
    std::cout << "test" << std::endl;
  }catch(std::runtime_error e){
    arduino = new Comm::Serial::FullStack(ARDUINO_PORT_EMULATED, ARDUINO_BAUD);
  }
  arduino->restartArduino();
  std::cout << "Connecting to network" << std::endl;
  agent = new Comm::TCP::FullStack(AGENT_PORT, AGENT_DELIMITER);
  std::cout << "Created Hubs" << std::endl;
}

void createSubsystems(){
  std::cout << "Creating subsystems" << std::endl;
  
  //arduino.hub()->on(1,[&agent](std::vector<int> message){
  //  bool enable = message.size()>0&&message.at(0)==1;
  //  agent.hub()->emit("killswitch", std::vector<std::string>{(enable?"1":"0")});
  //});
  
  arduino->hub()->on(ECHO_PORT_NUM, [](std::vector<int> message){
    std::cout << "Echo from arduino: " << message[0] << std::endl;
  });

  // arduino->hub()->on(4,[](std::vector<int> message){
  //   //std::cout << "Got IMU" << std::endl;
  //   if(message.size() != 3)
  //     return;
  //   agent->hub()->emit("imu/rotation", std::vector<std::string>{std::to_string(message[0]),std::to_string(message[1]),std::to_string(message[2])});
  // });
  
  arduino->hub()->emit(4, {1}); // enable imu output
  
  arduino->hub()->on(10,[](std::vector<int> message){
    std::cout << "Got IMU!" << std::endl;
    if(message.size() != 9)
      return;
    std::cout << "gyro: " << message[0] << ", " << message[1] << ", " << message[2] << ", " << std::endl;
    std::cout << "accel: " << message[3] << ", " << message[4] << ", " << message[5] << ", " << std::endl;
    std::cout << "magnet: " << message[6] << ", " << message[7] << ", " << message[8] << ", " << std::endl;
    //agent->hub()->emit("imu/rotation", std::vector<std::string>{std::to_string(message[0]),std::to_string(message[1]),std::to_string(message[2])});
  });

  dive = new Subsystem::Dive(arduino->hub(), DIVE_PORT_NUM, agent->hub(), DIVE_PORT_NAME);
  tank = new Subsystem::Tank(arduino->hub(), TANK_PORT_NUM, agent->hub(), TANK_PORT_NAME);
  std::cout << "Created subsystems" << std::endl;
}

void runEventLoop(){
  while(!stopApp){
    arduino->hub()->poll();
    agent->hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY));
  }
}
