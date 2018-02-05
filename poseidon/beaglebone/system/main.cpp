#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/TCP/FullStack.hpp>

#include <thread>
#include <chrono>

// int main(){
//   bool shouldExit = false;
//
//
//
//   raspberryPI.hub()->on(10, [&raspberryPI, &shouldExit](std::vector<int> message){
//     std::cout << "message from Arduino: ";
//     for(unsigned int i = 0; i < message.size(); ++i)
//       std::cout << "\"" << std::dec << message[i] << "\", ";
//     std::cout << std::endl;
//     if(message[0] > 500){
//       std::cout << "Performing restart on Arduino" << std::endl;
//       raspberryPI.restartArduino();
//     }
//   });
//
//   raspberryPI.hub()->on(0, [&raspberryPI, &shouldExit](std::vector<int> message){
//     //std::cout << "alignment from arduino" << std::endl;
//   });
//
//   while(!shouldExit){
//     //raspberryPI.hub()->emit(0, std::vector<int>{0});
//     raspberryPI.hub()->poll();
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//   }
//
//   return 0;
// }

int main(){
  bool shouldExit = false;

  //Comm::TCP::FullStack raspberryPI(3001, '|');
  Comm::Serial::FullStack arduino("/dev/ttyS6", B115200);
  arduino.restartArduino();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  // raspberryPI.hub()->on("echo", [&raspberryPI](std::vector<std::string> message){
  //   std::cout << "message from Raspberry PI: ";
  //   for(unsigned int i = 0; i < message.size(); ++i)
  //     std::cout << "\"" << std::dec << message[i] << "\", ";
  //   std::cout << std::endl;
  //   raspberryPI.hub()->emit("echo/r", message);
  // });
  //
  // raspberryPI.hub()->on("exit", [&shouldExit](std::vector<std::string> message){
  //   shouldExit = true;
  // });
  //
  arduino.hub()->on(10, [/*&raspberryPI, */&shouldExit](std::vector<int> message){
    std::cout << "message from Arduino: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << "\"" << std::dec << message[i] << "\", ";
    std::cout << std::endl;
    //raspberryPI.hub()->emit("arduino", std::vector<std::string>{std::to_string(message[0])});
  });

  int count = 0;
  arduino.hub()->on(42, [/*&raspberryPI, */&shouldExit, &count, &arduino](std::vector<int> message){
    std::cout << "Echo: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << "\"" << std::dec << message[i] << "\", ";
    std::cout << std::endl;
    if(message.size() != 1 || message[0] != count){
      std::cout << "^ ====== Wrong ===== ^" << std::endl;
    }
    count++;
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    arduino.hub()->emit(1, std::vector<int>{count});
    //raspberryPI.hub()->emit("arduino", std::vector<std::string>{std::to_string(message[0])});
  });

  arduino.hub()->emit(1, std::vector<int>{count});

  while(!shouldExit){
    //raspberryPI.hub()->poll();
    arduino.hub()->poll();
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
