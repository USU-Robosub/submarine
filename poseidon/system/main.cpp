#include <iostream>

#include <Comm/Serial/FullStack.hpp>
#include <Comm/TCP/FullStack.hpp>
#include <Vision/Livestream.hpp>
#include <Comm/tools.hpp>
#include <Subsystem/Dive.hpp>
#include <Subsystem/Tank.hpp>

#include <thread>
#include <chrono>
#include <string>

int main(){
  bool shouldExit = false;
  //Vision::Livestream vision2("1-1.2",8082,100);
  //Vision::Livestream vision3("1-1.3",8083,100);
  //Vision::Livestream vision4("1-1.4",8084,100);

///dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
//

  //Comm::Serial::FullStack arduino(Comm::Serial::Port::portNameFromPath("1.5"), B115200);
  Comm::Serial::FullStack arduino("/tmp/virtualcom1", B115200);
  arduino.restartArduino();
  Comm::TCP::FullStack agent(3001, '|');

  std::cout << "created network" << std::endl;

  int throttle = 90, steering = 90;//, dive = 90;

  //arduino.hub()->on(1,[&agent](std::vector<int> message){
  //  bool enable = message.size()>0&&message.at(0)==1;
  //  agent.hub()->emit("killswitch", std::vector<std::string>{(enable?"1":"0")});
  //});

  arduino.hub()->on(42,[](std::vector<int> message){
    std::cout << "echo " << message[0] << std::endl;
  });

  Subsystem::Dive dive(arduino.hub(), 2, agent.hub(), "dive");
  Subsystem::Tank tank(arduino.hub(), 3, agent.hub(), "tank");

  while(!shouldExit){
    arduino.hub()->poll();
    agent.hub()->poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
