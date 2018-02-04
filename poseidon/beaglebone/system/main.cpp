#include <iostream>

#include <Comm/Serial/FullStack.hpp>

#include <thread>
#include <chrono>

int main(){
  bool shouldExit = false;

  Comm::Serial::FullStack raspberryPI("/dev/ttyS6", B115200);

  raspberryPI.hub()->on(10, [&raspberryPI, &shouldExit](std::vector<int> message){
    std::cout << "message from Arduino: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << "\"" << std::dec << message[i] << "\", ";
    std::cout << std::endl;
  });

  raspberryPI.hub()->on(0, [&raspberryPI, &shouldExit](std::vector<int> message){
    //std::cout << "alignment from arduino" << std::endl;
  });

  while(!shouldExit){
    //raspberryPI.hub()->emit(0, std::vector<int>{0});
    raspberryPI.hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}

// int main(){
//   bool shouldExit = false;
//
//   Comm::TCP::FullStack raspberryPI(3001, '|');
//
//   raspberryPI.hub()->on("echo", [&raspberryPI](std::vector<std::string> message){
//     std::cout << "message from Raspberry PI: ";
//     for(unsigned int i = 0; i < message.size(); ++i)
//       std::cout << "\"" << std::dec << message[i] << "\", ";
//     std::cout << std::endl;
//     raspberryPI.hub()->emit("echo/r", message);
//   });
//
//   raspberryPI.hub()->on("exit", [&shouldExit](std::vector<std::string> message){
//     shouldExit = true;
//   });
//
//   while(!shouldExit){
//     raspberryPI.hub()->poll();
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//   }
//
//   return 0;
// }
