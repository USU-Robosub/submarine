#include <iostream>

#include <Comm/Hub.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>

#include <thread>
#include <chrono>

int main(){
  bool shouldExit = false;

  Comm::TCP::SingleClientServer server(3001);
  Comm::TCP::Port raspberryPort = server.waitForConnection();
  Comm::TCP::Stream raspberryStream(&raspberryPort, '|');
  Comm::TCP::Bridge raspberryBridge(&raspberryStream);
  Comm::Hub<std::string> raspberryPI(&raspberryBridge);

  raspberryPI.on("echo", [&raspberryPI](std::vector<std::string> message){
    std::cout << "message from Raspberry PI: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << "\"" << std::dec << message[i] << "\", ";
    std::cout << std::endl;
    raspberryPI.emit("echo/r", message);
  });

  raspberryPI.on("exit", [&shouldExit](std::vector<std::string> message){
    shouldExit = true;
  });

  while(!shouldExit){
    raspberryPI.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
