#include <catch.hpp>
#include <Comm/Serial/FullStack.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define TEST_PORT_1 "/tmp/virtualcom0"
#define TEST_PORT_2 "/tmp/virtualcom1"

TEST_CASE("Full stack test", "[SerialIntegration][.integration]"){
  Comm::Serial::FullStack sending(TEST_PORT_1, B9600);
  Comm::Serial::FullStack receiving(TEST_PORT_2, B9600);

  std::cout << "sending data..." << std::endl;

  // receiving.hub()->on(10, [](std::vector<int> message){
  //   std::cout << "message on 10: ";
  //   for(unsigned int i = 0; i < message.size(); ++i)
  //     std::cout << std::dec << message[i] << ' ';
  //   std::cout << std::endl;
  // });
  // sending.hub()->on(3, [](std::vector<int> message){
  //   std::cout << "message on 3: ";
  //   for(unsigned int i = 0; i < message.size(); ++i)
  //     std::cout << std::dec << message[i] << ' ';
  //   std::cout << std::endl;
  // });
  //
  // sending.hub()->emit(10, std::vector<int>{1, 2, 3, 4});
  // receiving.hub()->emit(3, std::vector<int>{0, 0, 400, 2, 5});
  // sending.hub()->emit(10, std::vector<int>{1000, 4, 2});
  // receiving.hub()->emit(3, std::vector<int>{-8, -6, 0, -9, -10, -100, -8765});

  int count = 0;

  receiving.hub()->on(10, [&count](std::vector<int> message){
    REQUIRE( message[0] <= count );
  });

  while(count < 1000){
    //td::this_thread::sleep_for(std::chrono::milliseconds(100));
    sending.hub()->emit(10, std::vector<int>{++count});
    receiving.hub()->poll();
    //sending.hub()->poll();
  }

  std::cout << "sent data" << std::endl;
}
