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

TEST_CASE("can connect to port", "[Serial][.]"){
  Comm::Serial::Port serial(TEST_PORT_1, B9600);
  serial.connect();
  REQUIRE( serial.isConnected() );
  serial.disconnect();
}

TEST_CASE("can disconnect and reconnect to port", "[Serial][.]"){
  Comm::Serial::Port serial(TEST_PORT_1, B9600);
  serial.connect();
  REQUIRE( serial.isConnected() );
  serial.disconnect();
  Comm::Serial::Port serial2(TEST_PORT_1, B9600);
  serial2.connect();
  REQUIRE( serial2.isConnected() );
  serial2.disconnect();
}

TEST_CASE("disconnects when leaving scope", "[Serial][.]"){
  {
    Comm::Serial::Port serial(TEST_PORT_1, B9600);
    serial.connect();
    REQUIRE( serial.isConnected() );
  }
  Comm::Serial::Port serial2(TEST_PORT_1, B9600);
  serial2.connect();
  REQUIRE( serial2.isConnected() );
  serial2.disconnect();
}

TEST_CASE("can't connect to non-existent port", "[Serial][.]"){
  Comm::Serial::Port serial(ERROR_TEST_PORT, B9600);
  serial.connect();
  REQUIRE( serial.isConnected() == false );
  serial.disconnect();
}

TEST_CASE("data is available after pushing data", "[Serial][.]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);
  sender.connect();
  receiver.connect();

  REQUIRE( receiver.hasData() == false );
  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); // give the OS some time
  REQUIRE( receiver.hasData() );

  sender.disconnect();
  receiver.disconnect();
}

TEST_CASE("data can be poll after being pushed", "[Serial][.]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);
  sender.connect();
  receiver.connect();

  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  unsigned char retrievedData[6] = { 0 };
  receiver.poll(retrievedData, length);
  for(unsigned int i = 0; i < length; i++)
    REQUIRE( data[i] == retrievedData[i] );

  sender.disconnect();
  receiver.disconnect();
}

TEST_CASE("does not push when unlocked", "[Serial][.]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);
  sender.connect();
  receiver.connect();

  REQUIRE( receiver.hasData() == false );
  sender.unlock();
  sender.push(data, length);
  sender.lock();
  sender.unlock();
  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); // give the OS some time
  REQUIRE( receiver.hasData() == false );

  sender.disconnect();
  receiver.disconnect();
}

TEST_CASE("does not poll when unlocked", "[Serial][.]"){
  unsigned char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned char buffer[6] = { 0 };
  unsigned short length = 6;
  Comm::Serial::Port sender(TEST_PORT_1, B9600);
  Comm::Serial::Port receiver(TEST_PORT_2, B9600);
  sender.connect();
  receiver.connect();

  sender.push(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  unsigned char retrievedData[6] = { 0 };
  receiver.unlock();
  receiver.poll(retrievedData, length);
  receiver.lock();
  receiver.unlock();
  receiver.poll(retrievedData, length);
  for(unsigned int i = 0; i < length; i++)
    REQUIRE( buffer[i] == retrievedData[i] );

  sender.disconnect();
  receiver.disconnect();
}
