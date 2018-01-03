#include <catch.hpp>
#include <Comm/TCP/Port.hpp>
#include <thread>
#include <chrono>

#define TEST_ADDRESS "127.0.0.1"
#define TEST_PORT 3001
#define IMPOSSIBLE_PORT 80

TEST_CASE("can host server", "[TCP_Port]"){
  tacopie::tcp_client client;
  Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
  port.host();

  SECTION("server is hosting"){
    client.connect(TEST_ADDRESS, TEST_PORT, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    REQUIRE( client.is_connected() );
    client.disconnect();
    port.disconnect();
  }

  SECTION("server has stopped hosting"){
    port.disconnect();
    REQUIRE_THROWS( client.connect(TEST_ADDRESS, TEST_PORT, 10) );
    REQUIRE_FALSE( client.is_connected() );
  }
}

TEST_CASE("can't host impossible server", "[TCP_Port]"){
  tacopie::tcp_client client;
  Comm::TCP::Port port(TEST_ADDRESS, IMPOSSIBLE_PORT);
  REQUIRE_NOTHROW( port.host() );
}

TEST_CASE("can connect to server", "[TCP_Port]"){
  bool didConnect = false;
  tacopie::tcp_server server;
  Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
  server.start(TEST_ADDRESS, TEST_PORT,
    [&didConnect] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
    didConnect = true;
    return true;
  });

  SECTION("server is hosting"){
    port.connect();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    REQUIRE( didConnect );
    port.disconnect();
    server.stop();
  }

  SECTION("server has stopped hosting"){
    server.stop();
    REQUIRE_NOTHROW( port.connect() );
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    REQUIRE_FALSE( didConnect );
  }
}

TEST_CASE("can send data", "[TCP_Port_1]"){
  Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
  Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
  SECTION("server -> client"){
    sender.host();
    receiver.connect();
  }
  SECTION("client -> server"){
    receiver.host();
    sender.connect();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  sender.push(std::string("test").c_str(), 4);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  char received[4];
  receiver.poll(received, 4);
  REQUIRE( std::string(received, 4) == std::string("test") );

  sender.disconnect();
  receiver.disconnect();
}
