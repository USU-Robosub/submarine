#include <catch.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/Port.mock.hpp>

TEST_CASE("tcp stream can read string", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, '|');
  char data[7] = "a|abc|";
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
  REQUIRE( std::string(buffer, 6) == std::string("a|abc|") );
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
    REQUIRE( std::string(buffer, 6) == std::string("a,abc,") );
  }
}

TEST_CASE("tcp stream can handle large buffers", "[TCP_Stream]"){
  Comm::Mock::Port<char> port;
  Comm::TCP::Stream stream(&port, '|', 10);

  char data[24] = "0|1|2|3|4444|5|6|7|8|9|";
  port.buffer = data;
  port.bufferLength = 20;
  REQUIRE( stream.poll() == std::string("0") );
  REQUIRE( stream.poll() == std::string("1") );
  REQUIRE( stream.poll() == std::string("2") );
  REQUIRE( stream.poll() == std::string("3") );
  REQUIRE( stream.poll() == std::string("4444") );
  REQUIRE( stream.poll() == std::string("5") );
  REQUIRE( stream.poll() == std::string("6") );
  REQUIRE( stream.poll() == std::string("7") );
  REQUIRE( stream.poll() == std::string("8") );
  REQUIRE( stream.poll() == std::string("9") );
}
