#include "catch.hpp"
#include "Comm/Serial.h"

/*
  NOTICE: This set of test casses are disabled
  by default because they require a virutal serial
  port and root permissions to run correctly.
*/

#define TEST_PORT "/dev/ttyS10"
#define OTHER_TEST_PORT "/dev/ttyS11"

TEST_CASE("can connect to port", "[Serial][hide]"){
  Comm::Serial serial(TEST_PORT, B9600);
  serial.connect();
  REQUIRE( serial.status() == Comm::Status::Ready );
  serial.disconnect();
}

TEST_CASE("serial can send and receive a message", "[Serial][hide]"){
  Comm::Serial sendingSerial(TEST_PORT, B9600);
  Comm::Serial receivingSerial(OTHER_TEST_PORT, B9600);
  sendingSerial.connect();
  receivingSerial.connect();

  char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;

  sendingSerial.send(data, length);
  REQUIRE( receivingSerial.hasPending() );

  receivingSerial.disconnect();
  sendingSerial.disconnect();
}

TEST_CASE("serial can send and receive a message", "[Serial][hide]"){
  Comm::Serial sendingSerial(TEST_PORT, B9600);
  Comm::Serial receivingSerial(OTHER_TEST_PORT, B9600);
  sendingSerial.connect();
  receivingSerial.connect();

  char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned short length = 6;

  sendingSerial.send(data, length);
  Comm::Data readData = receivingSerial.read();
  REQUIRE( readData.length == length );
  for(unsigned int i = 0; i < length; ++i){
    REQUIRE( readData.data[i] == data[i] );
  }

  delete[] readData.data;
  receivingSerial.disconnect();
  sendingSerial.disconnect();
}
