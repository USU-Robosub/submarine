#include <catch.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/Port.mock.hpp>

TEST_CASE("tcp stream can read string", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, '|');
  char data[] = "a|abc|";
  port.buffer = data;
  port.bufferLength = 6;
  REQUIRE( stream.poll() == std::string("a") );
  REQUIRE( stream.poll() == std::string("abc") );
}

TEST_CASE("tcp stream can write int", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, '|');
  char buffer[6] = { ' ' };
  port.buffer = buffer;
  port.bufferLength = 6;
  stream.push("a");
  stream.push("abc");
  REQUIRE( buffer == std::string("a|abc|") );
}

TEST_CASE("tcp stream has data when queue is not empty", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, '|');
  char data[] = "test|message|";
  port.buffer = data;
  REQUIRE_FALSE( stream.hasData() );
  port.bufferLength = 13;
  REQUIRE( stream.hasData() );
  stream.poll();
  REQUIRE( stream.hasData() );
  stream.poll();
  REQUIRE_FALSE( stream.hasData() );
}

TEST_CASE("tcp stream can use different separators", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, ',');

  SECTION("read"){
    char data[] = "a,abc,";
    port.buffer = data;
    port.bufferLength = 6;
    REQUIRE( stream.poll() == std::string("a") );
    REQUIRE( stream.poll() == std::string("abc") );
  }

  SECTION("write"){
    char buffer[6] = { ' ' };
    port.buffer = buffer;
    port.bufferLength = 6;
    stream.push("a");
    stream.push("abc");
    REQUIRE( buffer == std::string("a,abc,") );
  }
}
