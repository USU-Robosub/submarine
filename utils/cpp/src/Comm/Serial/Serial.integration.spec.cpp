#include <catch.hpp>
#include <Comm/Hub.hpp>
#include <Comm/Serial/Bridge.hpp>
#include <Comm/Serial/Stream.hpp>
#include <Comm/Serial/Port.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define TEST_PORT_1 "/dev/ttyS10"
#define TEST_PORT_2 "/dev/ttyS11"

TEST_CASE("Full stack test", "[SerialIntegration][.]"){
  Comm::Serial::Port sendingPort(TEST_PORT_1, B9600); sendingPort.unlock();
  sendingPort.connect();
  REQUIRE( sendingPort.isConnected() );
  Comm::Serial::Stream sendingStream(&sendingPort);
  Comm::Serial::Bridge sendingBridge(&sendingStream);
  Comm::Hub<int> sendingHub(&sendingBridge);

  Comm::Serial::Port receivingPort(TEST_PORT_2, B9600); receivingPort.unlock();
  receivingPort.connect();
  REQUIRE( receivingPort.isConnected() );
  Comm::Serial::Stream receivingStream(&receivingPort);
  Comm::Serial::Bridge receivingBridge(&receivingStream);
  Comm::Hub<int> receivingHub(&receivingBridge);

  receivingHub.on(10, [](std::vector<int> message){
    std::cout << "message on 10: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << std::dec << message[i] << ' ';
    std::cout << std::endl;
  });
  sendingHub.on(3, [](std::vector<int> message){
    std::cout << "message on 3: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << std::dec << message[i] << ' ';
    std::cout << std::endl;
  });

  sendingHub.emit(10, std::vector<int>{1, 2, 3, 4});
  receivingHub.emit(3, std::vector<int>{0, 0, 400, 2, 5});
  sendingHub.emit(10, std::vector<int>{1000, 4, 2});
  receivingHub.emit(3, std::vector<int>{-8, -6, 0, -9, -10, -100, -8765});

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  receivingHub.poll();
  sendingHub.poll();
}
