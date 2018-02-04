#include <iostream>

#include <Comm/TCP/FullStack.hpp>

#include <thread>
#include <chrono>

int main(){
  bool shouldExit = false;

  Comm::TCP::FullStack raspberryPI(3001, '|');

  raspberryPI.hub()->on("echo", [&raspberryPI](std::vector<std::string> message){
    std::cout << "message from Raspberry PI: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << "\"" << std::dec << message[i] << "\", ";
    std::cout << std::endl;
    raspberryPI.hub()->emit("echo/r", message);
  });

  raspberryPI.hub()->on("exit", [&shouldExit](std::vector<std::string> message){
    shouldExit = true;
  });

  while(!shouldExit){
    raspberryPI.hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
