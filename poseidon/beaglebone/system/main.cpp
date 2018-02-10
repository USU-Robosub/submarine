#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/TCP/FullStack.hpp>

#include <thread>
#include <chrono>
#include <string>

int main(){
  bool shouldExit = false;

  Comm::Serial::FullStack arduino("/dev/ttyS8", B115200);
  arduino.restartArduino();
  Comm::TCP::FullStack agent(3001, '|');

  int throttle = 90, steering = 90, dive = 90;

  agent.hub()->on("throttle", [&throttle, &steering, &arduino](std::vector<std::string> message){
    throttle = std::stoi(message[0]);
    arduino.hub()->emit(1, std::vector<int>{throttle, steering});
  });

  agent.hub()->on("steering", [&throttle, &steering, &arduino](std::vector<std::string> message){
    steering = std::stoi(message[0]);
    arduino.hub()->emit(1, std::vector<int>{throttle, steering});
  });

  agent.hub()->on("dive", [&dive, &arduino](std::vector<std::string> message){
    dive = std::stoi(message[0]);
    arduino.hub()->emit(1, std::vector<int>{dive});
  });

  while(!shouldExit){
    arduino.hub()->poll();
    agent.hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
