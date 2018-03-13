#include <catch.hpp>
#include <Comm/Serial/Stream.hpp>
#include <Comm/Port.mock.hpp>
#include <thread>
#include <chrono>

TEST_CASE("serial stream can read int", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  port.bufferLength = 8;
  REQUIRE( stream.poll() == 67305985 );
  REQUIRE( stream.poll() == 16909060 );
}

TEST_CASE("serial stream can write int", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  unsigned char buffer[8] = { 0 };
  port.buffer = buffer;
  port.bufferLength = 8;
  stream.push(67305985);
  stream.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}

TEST_CASE("serial stream uses port to check if data is available", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  REQUIRE_FALSE( stream.hasData() );
  port.bufferLength = 8;
  REQUIRE( stream.hasData() );
  stream.poll();
  REQUIRE( stream.hasData() );
  stream.poll();
  REQUIRE_FALSE( stream.hasData() );
}
