#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/TCP/FullStack.hpp>
#include <Vision/Livestream.hpp>

#include <thread>
#include <chrono>
#include <string>

int main(){
  bool shouldExit = false;
  Vision::Livestream vision2("1-1.2",8082,100);
  Vision::Livestream vision3("1-1.3",8083,100);
  Vision::Livestream vision4("1-1.4",8084,100);

///dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
//
  Comm::Serial::FullStack arduino(Comm::Serial::Port::portNameFromPath("1.5"), B115200);
  arduino.restartArduino();
  Comm::TCP::FullStack agent(3001, '|');

  int throttle = 90, steering = 90, dive = 90;
  
  arduino.hub()->on(1,[&agent](std::vector<int> message){
    bool enable = message.size()>0&&message.at(0)==1;
    agent.hub()->emit("killswitch", std::vector<std::string>{(enable?"1":"0")});
  });

  agent.hub()->on("throttle", [&throttle, &steering, &arduino](std::vector<std::string> message){
    throttle = std::stoi(message[0]);
    arduino.hub()->emit(2, std::vector<int>{throttle, steering});
  });

  agent.hub()->on("steering", [&throttle, &steering, &arduino](std::vector<std::string> message){
    steering = std::stoi(message[0]);
    arduino.hub()->emit(2, std::vector<int>{throttle, steering});
  });

  agent.hub()->on("dive", [&dive, &arduino](std::vector<std::string> message){
    dive = std::stoi(message[0]);
    arduino.hub()->emit(3, std::vector<int>{dive});
  });

  while(!shouldExit){
    arduino.hub()->poll();
    agent.hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
