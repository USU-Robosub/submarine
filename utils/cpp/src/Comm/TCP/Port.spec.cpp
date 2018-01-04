#include <catch.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <thread>
#include <chrono>

#define TEST_ADDRESS "localhost"
#define TEST_PORT 3001
#define PAUSE std::this_thread::sleep_for(std::chrono::milliseconds(10));

TEST_CASE("tcp port throws ConnectionFailure", "[TCP_Port]"){
  REQUIRE_THROWS_AS( Comm::TCP::Port(TEST_ADDRESS, TEST_PORT), Comm::TCP::ConnectionFailure );
}

#include <iostream>
TEST_CASE("tcp port connects to server", "[TCP_Port]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
  server.waitForConnection();
  REQUIRE( true );
}

TEST_CASE("tcp port can send data", "[TCP_Port]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
  Comm::TCP::Port receiver(server.waitForConnection());
  PAUSE
  REQUIRE_FALSE( receiver.hasData() );
  sender.push(std::string("test").c_str(), 4);
  PAUSE
  REQUIRE( receiver.hasData() );
}

TEST_CASE("tcp port can read data", "[TCP_Port]"){
  Comm::TCP::SingleClientServer server(TEST_PORT);
  PAUSE
  Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
  Comm::TCP::Port receiver(server.waitForConnection());
  sender.push(std::string("test").c_str(), 4);
  PAUSE
  char received[4];
  receiver.poll(received, 4);
  REQUIRE( std::string(received, 4) == std::string("test") );
}
//
// /*
//   Travic-CI is very sensitive to this.
// */
// TEST_CASE("tcp port disconnects when it goes out of scope", "[TCP_Port]"){
//   {
//     Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//     port.host();
//     Comm::TCP::Port port2(TEST_ADDRESS, TEST_PORT);
//     REQUIRE_THROWS_AS( port2.host(), Comm::TCP::ConnectionFailure );
//   }
//   Comm::TCP::Port port2(TEST_ADDRESS, TEST_PORT);
//   REQUIRE_NOTHROW( port2.host() );
// }
//
// TEST_CASE("can host server", "[TCP_Port]"){
//   tacopie::tcp_client client;
//   Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//   port.host();
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
// TEST_CASE("can connect to server", "[TCP_Port]"){
//   bool didConnect = false;
//   tacopie::tcp_server server;
//   Comm::TCP::Port port(TEST_ADDRESS, TEST_PORT);
//   server.start(TEST_ADDRESS, TEST_PORT,
//     [&didConnect] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
//     didConnect = true;
//     return true;
//   });
//   port.connect();
//   PAUSE
//   REQUIRE( didConnect );
//   port.disconnect();
//   server.stop();
// }
//
// TEST_CASE("can send data", "[TCP_Port]"){
//   Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
//   Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
//   SECTION("server -> client"){
//     sender.host();
//     receiver.connect();
//     sender.wait();
//   }
//   SECTION("client -> server"){
//     receiver.host();
//     sender.connect();
//     receiver.wait();
//   }
//
//   sender.push(std::string("test").c_str(), 4);
//   char received[4];
//   receiver.poll(received, 4);
//   REQUIRE( std::string(received, 4) == std::string("test") );
//
//   sender.disconnect();
//   receiver.disconnect();
// }
//
// TEST_CASE("has data", "[TCP_Port]"){
//   Comm::TCP::Port sender(TEST_ADDRESS, TEST_PORT);
//   Comm::TCP::Port receiver(TEST_ADDRESS, TEST_PORT);
//   SECTION("server -> client"){
//     sender.host();
//     receiver.connect();
//     sender.wait();
//   }
//   SECTION("client -> server"){
//     receiver.host();
//     sender.connect();
//     receiver.wait();
//   }
//
//   REQUIRE_FALSE( receiver.hasData() );
//   sender.push(std::string("test").c_str(), 4);
//   REQUIRE( receiver.hasData() );
//
//   sender.disconnect();
//   receiver.disconnect();
// }
