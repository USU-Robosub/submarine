#include <catch.hpp>
#include <Comm/TCP/Port.hpp>
#include <thread>
#include <chrono>

#define TEST_ADDRESS "localhost"
#define TEST_PORT 3001
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(10));

TEST_CASE("tcp port throws ConnectionFailure", "[TCP_Port]"){
  SECTION("when unable to host"){
    Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
    Comm::TCP::Port port2(TEST_ADDRESS, TEST_PORT);
    port.host();
    PAUSE
    REQUIRE_THROWS_AS( port2.host(), Comm::TCP::ConnectionFailure );
  }

  SECTION("when unable to connect"){
    Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
    REQUIRE_THROWS_AS( port.connect(), Comm::TCP::ConnectionFailure );
  }
}

/*
  Travic-CI is very sensitive to this.
*/
TEST_CASE("tcp port disconnects when it goes out of scope", "[TCP_Port]"){
  {
    Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
    port.host();
    PAUSE
  }
  PAUSE
  Comm::TCP::Port port2(TEST_ADDRESS, TEST_PORT);
  REQUIRE_NOTHROW( port2.host() );
}

// TEST_CASE("can host server", "[TCP_Port]"){
//   tacopie::tcp_client client;
//   Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//   CHECK( port.host() );
//   PAUSE
//
//   SECTION("server is hosting"){
//     client.connect(TEST_ADDRESS, TEST_PORT, 10);
//     REQUIRE( client.is_connected() );
//     client.disconnect();
//     port.disconnect();
//   }
//
//   SECTION("server has stopped hosting"){
//     port.disconnect();
//     REQUIRE_THROWS( client.connect(TEST_ADDRESS, TEST_PORT, 10) );
//     REQUIRE_FALSE( client.is_connected() );
//   }
// }
//
// TEST_CASE("can't host server on serving port", "[TCP_Port]"){
//   tacopie::tcp_server server;
//   Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//   server.start(TEST_ADDRESS, TEST_PORT,
//     [] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
//     return true;
//   });
//   REQUIRE_NOTHROW( port.host() );
// }
//
// TEST_CASE("can connect to server", "[TCP_Port]"){
//   bool didConnect = false;
//   tacopie::tcp_server server;
//   Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//   server.start(TEST_ADDRESS, TEST_PORT,
//     [&didConnect] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
//     didConnect = true;
//     return true;
//   });
//
//   SECTION("server is hosting"){
//     port.connect();
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     REQUIRE( didConnect );
//     port.disconnect();
//     server.stop();
//   }
//
//   SECTION("server has stopped hosting"){
//     server.stop();
//     REQUIRE_NOTHROW( port.connect() );
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     REQUIRE_FALSE( didConnect );
//   }
// }
//
// TEST_CASE("can send data", "[TCP_Port_1]"){
//   Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
//   Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
//   SECTION("server -> client"){
//     sender.host();
//     receiver.connect();
//   }
//   SECTION("client -> server"){
//     receiver.host();
//     sender.connect();
//   }
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//   sender.push(std::string("test").c_str(), 4);
//   std::this_thread::sleep_for(std::chrono::milliseconds(10));
//   char received[4];
//   receiver.poll(received, 4);
//   REQUIRE( std::string(received, 4) == std::string("test") );
//
//   sender.disconnect();
//   receiver.disconnect();
// }
