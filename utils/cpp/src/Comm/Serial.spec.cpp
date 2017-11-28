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
  REQUIRE( serial.connect() );
  serial.disconnect();
}

TEST_CASE("serial can send and receive a message", "[Serial][hide]"){
  Comm::Serial sendingSerial(TEST_PORT, B9600);
  Comm::Serial receivingSerial(OTHER_TEST_PORT, B9600);
  sendingSerial.connect();
  receivingSerial.connect();

char* data
unsigned int length

  sendingSerial.send(data, length);
  Comm::Serial::Data readData = sendingSerial.read();
  REQUIRE( readData.length == data.length );
  REQUIRE( readData.data == data.data );

  receivingSerial.disconnect();
  sendingSerial.disconnect();
}
