#include <catch.hpp>
#include <Comm/Serial/Stream.hpp>
#include <Comm/BinaryPort.mock.hpp>
#include <thread>
#include <chrono>

TEST_CASE("can read int", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  REQUIRE( stream.poll() == 67305985 );
  REQUIRE( stream.poll() == 16909060 );
}

TEST_CASE("can write int", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  unsigned char buffer[8] = { 0 };
  port.buffer = buffer;
  stream.push(67305985);
  stream.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}

TEST_CASE("uses port to check if data is available", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);

  port.dataAvailable = false;
  REQUIRE( stream.hasData() == false );
  port.dataAvailable = true;
  REQUIRE( stream.hasData() == true );
}

TEST_CASE("uses port to lock/unlock stream", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);

  SECTION("lock"){
    REQUIRE( port.locked );
    port.locked = false;
    stream.lock();
    REQUIRE( port.locked );
  }

  SECTION("unlock"){
    port.unlocked = false;
    stream.unlock();
    REQUIRE( port.unlocked );
  }
}

TEST_CASE("does not read int when unlocked", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;

  stream.unlock();
  REQUIRE( stream.poll() == 0 );
  stream.lock();
  stream.unlock();
  REQUIRE( stream.poll() == 0 );
}

TEST_CASE("does not write int when unlocked", "[SerialStream]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::Stream stream(&port);
  unsigned char data[8] = { 0 };
  unsigned char buffer[8] = { 0 };
  port.buffer = data;

  stream.unlock();
  stream.push(67305985);
  stream.lock();
  stream.unlock();
  stream.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}
