#include <thread>
#include <chrono>
#include <catch.hpp>
#include <Comm/Serial/Port.hpp>

#include <iostream>

/*
  NOTICE: This set of test casses are disabled
  by default because they require a virtual serial
  port to run correctly.

  socat PTY,link=/tmp/virtualcom0 PTY,link=/tmp/virtualcom1
*/

#define TEST_PORT_1 "/tmp/virtualcom0"
#define TEST_PORT_2 "/tmp/virtualcom1"
#define ERROR_TEST_PORT "/tmp/virtualcom2"

TEST_CASE("serial port can connect to port", "[Serial][.]"){
  REQUIRE_NOTHROW( Comm::Serial::Port(TEST_PORT_1, B9600) );
}

TEST_CASE("serial port disconnects when leaving scope", "[Serial][.]"){
  {
    REQUIRE_NOTHROW( Comm::Serial::Port(TEST_PORT_1, B9600) );
  }
  REQUIRE_NOTHROW( Comm::Serial::Port(TEST_PORT_1, B9600) );
}

TEST_CASE("serial port can't connect to non-existent port", "[Serial][.]"){
  REQUIRE_THROWS_AS( Comm::Serial::Port(ERROR_TEST_PORT, B9600), Comm::ConnectionFailure);
}

TEST_CASE("serial port data is available after pushing data", "[Serial][.timing]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);

  REQUIRE( receiver.hasData() == false );
  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); // give the OS some time
  REQUIRE( receiver.hasData() );
}

TEST_CASE("serial port data can be poll after being pushed", "[Serial][.timing]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);

  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  unsigned char retrievedData[6] = { 0 };
  receiver.poll(retrievedData, length);
  for(unsigned int i = 0; i < length; i++)
    REQUIRE( data[i] == retrievedData[i] );
}
