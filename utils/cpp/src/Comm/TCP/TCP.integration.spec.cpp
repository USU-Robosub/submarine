#include <catch.hpp>
#include <Comm/Hub.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <Comm/TCP/FullStack.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define TEST_ADDRESS "localhost"
#define TEST_PORT 3001

TEST_CASE("TCP full stack test", "[TCP_Integration][.integration]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  Comm::TCP::FullStack receiving(TEST_ADDRESS, TEST_PORT, '|');
  Comm::TCP::FullStack sending(server.waitForConnection(), '|');

  receiving.hub()->on("test", [](std::vector<std::string> message){
    std::cout << "message on test: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << std::dec << message[i] << '_';
    std::cout << std::endl;
  });
  sending.hub()->on("event", [](std::vector<std::string> message){
    std::cout << "message on event: ";
    for(unsigned int i = 0; i < message.size(); ++i)
      std::cout << std::dec << message[i] << '_';
    std::cout << std::endl;
  });

  std::cout << "sending data..." << std::endl;

  sending.hub()->emit("test", std::vector<std::string>{"a", "b", "c", "d"});
  receiving.hub()->emit("event", std::vector<std::string>{"0", "0", "400", "2", "5"});
  sending.hub()->emit("test", std::vector<std::string>{"hello", ",", "world", "!"});
  receiving.hub()->emit("event", std::vector<std::string>{"this", "is", "a", "test"});

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  receiving.hub()->poll();
  sending.hub()->poll();

  std::cout << "data read..." << std::endl;
}
