#include <catch.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <thread>
#include <chrono>

#define TEST_ADDRESS "localhost"
#define TEST_PORT 3001
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(10));

/*
  Default timeout takes a long time to throw the exeption slowing down the tests.
*/
TEST_CASE("tcp port throws ConnectionFailure", "[TCP_Port][.timing]"){
  REQUIRE_THROWS_AS( Comm::TCP::Port(TEST_ADDRESS, TEST_PORT), Comm::TCP::ConnectionFailure );
}

TEST_CASE("tcp port connects to server", "[TCP_Port][.timing]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
  server.waitForConnection();
  REQUIRE( true );
}

TEST_CASE("tcp port can send data", "[TCP_Port][.timing]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  SECTION("client -> server"){
    Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
    Comm::TCP::Port receiver(server.waitForConnection());
    PAUSE
    REQUIRE_FALSE( receiver.hasData() );
    sender.push(std::string("test").c_str(), 4);
    PAUSE
    REQUIRE( receiver.hasData() );
  }
  SECTION("server -> client"){
    Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
    Comm::TCP::Port sender(server.waitForConnection());
    PAUSE
    REQUIRE_FALSE( receiver.hasData() );
    sender.push(std::string("hello").c_str(), 4);
    PAUSE
    REQUIRE( receiver.hasData() );
  }
}

TEST_CASE("tcp port can read data", "[TCP_Port][.timing]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  SECTION("client -> server"){
    Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
    Comm::TCP::Port receiver(server.waitForConnection());
    sender.push(std::string("test").c_str(), 4);
    PAUSE
    char received[4];
    receiver.poll(received, 4);
    REQUIRE( std::string(received, 4) == std::string("test") );
  }
  SECTION("server -> client"){
    Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
    Comm::TCP::Port sender(server.waitForConnection());
    sender.push(std::string("hello").c_str(), 5);
    PAUSE
    char received[5];
    receiver.poll(received, 5);
    REQUIRE( std::string(received, 5) == std::string("hello") );
  }
}
