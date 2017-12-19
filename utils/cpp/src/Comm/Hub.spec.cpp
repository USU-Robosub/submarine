#include <catch.hpp>
#include <Comm/Hub.hpp>
#include <Comm/Bridge.mock.hpp>
#include <string>
#include <vector>

typedef std::vector<std::string> vectString;

TEST_CASE("can emit messages", "[Hub]"){
  Comm::Mock::StringBridge bridge;
  Comm::Hub<std::string> hub(&bridge);

  SECTION("simple message"){
    hub.emit("test", vectString{"message"});
    REQUIRE( bridge.sent.size() == 1 );
    REQUIRE( bridge.sent[0] == vectString{"message", "test"} );
  }

  SECTION("complex message"){
    hub.emit("event", vectString{"data1", "data2", "data3", "data4"});
    REQUIRE( bridge.sent.size() == 1 );
    REQUIRE( bridge.sent[0] == vectString{"data1", "data2", "data3", "data4", "event"} );
  }
}

//TEST_CASE("ca")

// TEST_CASE("can emit simple message int", "[Hub]"){
//   Comm::Mock::IntBridge bridge;
//   Comm::Hub<int> hub(&bridge);
//   hub.emit(0, std::vector<int>{1, 2, 3});
//   hub.on(0, [](std::vector<int> message){
//     REQUIRE( message.size() == 3 );
//     REQUIRE( message[0] == 1 );
//     REQUIRE( message[1] == 2 );
//     REQUIRE( message[2] == 3 );
//   });
//   REQUIRE( bridge.sent.size() == 1 );
//   REQUIRE( bridge.sent[0] == std::vector<int>{0, 1, 2, 3} );
// }
