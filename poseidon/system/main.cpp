#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/Serial/PortFinder.hpp>
#include <Comm/TCP/FullStack.hpp>
#include <Vision/Livestream.hpp>
#include <Comm/tools.hpp>
#include <Subsystem/Dive.hpp>
#include <Subsystem/Tank.hpp>
#include <Comm/tools.hpp>
#include <sstream>

#include "settings.hpp"

#include <thread>
#include <chrono>
#include <cmath>
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
    if(message.size() >= 8 && message[0] == 0 && message[1] == 1 && message[2] == 2 && message[3] == 3 && message[4] == 4){
      unsigned int level = message[5];
      unsigned int compressedMessageLength = message[6];
      unsigned int messageLength = message[7];
      
      std::stringstream ss;
      for(unsigned int i = 0; i < messageLength; i++){
        char c = 0xFF & (message[8 + (i / 4)] >> ((i % 4) * 8));
        ss << c;
      }
      
      std::vector<std::string> stringMessage;
      stringMessage.push_back(std::to_string(level));
      stringMessage.push_back(ss.str());
      for(unsigned int i = (8 + compressedMessageLength); i < message.size(); i++){
        stringMessage.push_back(std::to_string(message[i]));
      }
      std::cout << "Log: ";
      for(unsigned int i = 0; i < stringMessage.size(); i++){
        std::cout << stringMessage[i] << ", ";
      }
      std::cout << std::endl;
      
      agent->hub()->emit("echo/arduino", stringMessage);
    }else{
      std::cout << "Echo from arduino: ";
      for(unsigned int i = 0; i < message.size(); i++){
        std::cout << message[i] << ", ";
      }
      std::cout << std::endl;
    }
  });

  arduino->hub()->on(101,[](std::vector<int> message){ // listen to kill switch
    if(message.size() >= 1){
      std::cout << "Kill swich: " << (message[0] == 1 ? "on" : "off") << std::endl;
      agent->hub()->emit("killSwitch/status", std::vector<std::string>{std::to_string(message[0])});
    }
  });
  
  arduino->hub()->emit(4, {1}); // enable imu output
  
  arduino->hub()->on(10,[](std::vector<int> message){
    //std::cout << "Got IMU!" << std::endl;
    if(message.size() != 18)
      return;
    // double gyro = std::sqrt(std::pow(message[0], 2) + std::pow(message[1], 2) + std::pow(message[2], 2)) / 1000000.0;
    // double accel = std::sqrt(std::pow(message[3], 2) + std::pow(message[4], 2) + std::pow(message[5], 2)) / 1000000.0;
    // std::cout << "gyro: " << message[0] << ", " << message[1] << ", " << message[2] << " -> " << gyro << std::endl;
    // std::cout << "accel: " << message[3] << ", " << message[4] << ", " << message[5] << " -> " << accel << std::endl;
    // std::cout << "magnet: " << message[6] << ", " << message[7] << ", " << message[8] << ", " << std::endl;
    agent->hub()->emit("imu/data", std::vector<std::string>{
      std::to_string(message[0]),std::to_string(message[1]),std::to_string(message[2]),
      std::to_string(message[3]),std::to_string(message[4]),std::to_string(message[5]),
      std::to_string(message[6]),std::to_string(message[7]),std::to_string(message[8]),
      std::to_string(message[9]),std::to_string(message[11]),std::to_string(message[12]),
      std::to_string(message[12]),std::to_string(message[13]),std::to_string(message[14]),
      std::to_string(message[15]),std::to_string(message[16]),std::to_string(message[17])
    });
  });
  
  arduino->hub()->on(1000,[](std::vector<int> message){
    std::cout << "Got Magnetic Calibration!" << std::endl;
    if(message.size() != 6)
      return;
    std::cout << "offset: " << message[0] << ", " << message[1] << ", " << message[2] << std::endl;
    std::cout << "scale: " << Comm::int32AsFloat(message[3]) << ", " << Comm::int32AsFloat(message[4]) << ", " << Comm::int32AsFloat(message[5]) << std::endl;
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
