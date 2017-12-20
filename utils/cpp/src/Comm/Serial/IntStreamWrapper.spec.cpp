#include <catch.hpp>
#include <Comm/Serial/IntStreamWrapper.hpp>
#include <Comm/BinaryPort.mock.hpp>
#include <thread>
#include <chrono>

TEST_CASE("can read int", "[IntStreamWrapper]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::IntStreamWrapper hub(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;
  REQUIRE( hub.poll() == 67305985 );
  REQUIRE( hub.poll() == 16909060 );
}

TEST_CASE("can write int", "[IntStreamWrapper]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::IntStreamWrapper hub(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  unsigned char buffer[8] = { 0 };
  port.buffer = buffer;
  hub.push(67305985);
  hub.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}

TEST_CASE("uses port to lock/unlock stream", "[IntStreamWrapper]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::IntStreamWrapper hub(&port);

  SECTION("lock"){
    REQUIRE( port.locked );
    port.locked = false;
    hub.lock();
    REQUIRE( port.locked );
  }

  SECTION("unlock"){
    port.unlocked = false;
    hub.unlock();
    REQUIRE( port.unlocked );
  }
}

TEST_CASE("does not read int when unlocked", "[IntStreamWrapper]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::IntStreamWrapper hub(&port);
  unsigned char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  port.buffer = data;

  hub.unlock();
  REQUIRE( hub.poll() == 0 );
  hub.lock();
  hub.unlock();
  REQUIRE( hub.poll() == 0 );
}

TEST_CASE("does not write int when unlocked", "[IntStreamWrapper]"){
  Comm::Mock::BinaryPort port;
  Comm::Serial::IntStreamWrapper hub(&port);
  unsigned char data[8] = { 0 };
  unsigned char buffer[8] = { 0 };
  port.buffer = data;

  hub.unlock();
  hub.push(67305985);
  hub.lock();
  hub.unlock();
  hub.push(16909060);
  for(unsigned int i = 0; i < 8; i++)
    REQUIRE( data[i] == buffer[i] );
}
