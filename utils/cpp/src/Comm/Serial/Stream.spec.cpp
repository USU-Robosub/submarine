#include <catch.hpp>
#include <Comm/Serial/Stream.hpp>
#include <Comm/Port.mock.hpp>
#include <thread>
#include <chrono>

TEST_CASE("can read int", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  port.bufferLength = 8;
  REQUIRE( stream.poll() == 67305985 );
  REQUIRE( stream.poll() == 16909060 );
}

TEST_CASE("can write int", "[SerialStream]"){
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

TEST_CASE("uses port to check if data is available", "[SerialStream]"){
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

TEST_CASE("uses port to lock/unlock stream", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);

  SECTION("lock"){
    REQUIRE( port.isLocked );
    port.isLocked = false;
    stream.lock();
    REQUIRE( port.isLocked );
  }

  SECTION("unlock"){
    port.isLocked = true;
    stream.unlock();
    REQUIRE_FALSE( port.isLocked );
  }
}

TEST_CASE("does not read int when unlocked", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  port.bufferLength = 8;

  stream.unlock();
  REQUIRE( stream.poll() == 0 );
  stream.lock();
  stream.unlock();
  REQUIRE( stream.poll() == 0 );
}

TEST_CASE("does not write int when unlocked", "[SerialStream]"){
  Comm::Mock::Port<unsigned char> port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[8] = { 0 };
  unsigned char buffer[8] = { 0 };
  port.buffer = data;
  port.bufferLength = 8;

  stream.unlock();
  stream.push(67305985);
  stream.lock();
  stream.unlock();
  stream.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}
