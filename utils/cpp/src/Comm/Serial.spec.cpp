#include <thread>
#include <chrono>
#include "catch.hpp"
#include "Comm/Serial.h"

#include <iostream>

/*
  NOTICE: This set of test casses are disabled
  by default because they require a virtual serial
  port and root permissions to run correctly.

  sudo socat PTY,link=/dev/ttyS10 PTY,link=/dev/ttyS11
*/

#define TEST_PORT_1 "/dev/ttyS10"
#define TEST_PORT_2 "/dev/ttyS11"
#define ERROR_TEST_PORT "/dev/ttyS20"

TEST_CASE("returns status ready after calling connect", "[Serial][hide]"){
  Comm::Serial serial(TEST_PORT_1, B9600);
  REQUIRE( serial.status() == Comm::Status::Disconnected );
  serial.connect();
  REQUIRE( serial.status() == Comm::Status::Ready );
  serial.disconnect();
  REQUIRE( serial.status() == Comm::Status::Disconnected );
}

TEST_CASE("returns status error after connecting to non-existent port", "[Serial][hide]"){
  Comm::Serial serial(ERROR_TEST_PORT, B9600);
  REQUIRE( serial.status() == Comm::Status::Disconnected );
  serial.connect();
  REQUIRE( serial.status() == Comm::Status::Error );
  serial.disconnect();
  REQUIRE( serial.status() == Comm::Status::Error );
}

TEST_CASE("data is available after sending data", "[Serial][hide]"){
  Comm::Serial sendingSerial(TEST_PORT_1, B9600);
  Comm::Serial readingSerial(TEST_PORT_2, B9600);
  sendingSerial.connect();
  readingSerial.connect();

  REQUIRE( readingSerial.canRead() == false );
  char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned int length = 6;
  sendingSerial.send(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  REQUIRE( readingSerial.canRead() );

  sendingSerial.disconnect();
  readingSerial.disconnect();
}

TEST_CASE("data can be read after it is sent", "[Serial][hide]"){
  Comm::Serial sendingSerial(TEST_PORT_1, B9600);
  Comm::Serial readingSerial(TEST_PORT_2, B9600);
  sendingSerial.connect();
  readingSerial.connect();

  char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned int length = 6;
  sendingSerial.send(data, length);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  char data2[] = { 0, 0, 0, 0, 0, 0 };
  readingSerial.receive(data2, length);

  for(unsigned int i = 0; i < length; i++){
    REQUIRE( data[i] == data2[i] );
  }

  sendingSerial.disconnect();
  readingSerial.disconnect();
}
