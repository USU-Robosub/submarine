#include <catch.hpp>
#include <Comm/Hub.hpp>
#include <Comm/Bridge.mock.hpp>
#include <string>

typedef std::vector<std::string> vectString;

TEST_CASE("can emit messages", "[Hub]"){
  Comm::Mock::Bridge<std::string> bridge;
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

TEST_CASE("can receive messages", "[Hub]"){
  Comm::Mock::Bridge<std::string> bridge;
  Comm::Hub<std::string> hub(&bridge);

  SECTION("simple message"){
    bridge.received.push(vectString{"message", "test"});
    vectString receivedMessage;
    hub.on("test", [&receivedMessage](vectString message){
      receivedMessage = message;
    });
    hub.poll();
    REQUIRE( receivedMessage == vectString{"message"} );
  }

  SECTION("complex message"){
    bridge.received.push(vectString{"data3", "data2", "data1", "event"});
    vectString receivedMessage;
    hub.on("event", [&receivedMessage](vectString message){
      receivedMessage = message;
    });
    hub.poll();
    REQUIRE( receivedMessage == vectString{"data3", "data2", "data1"} );
  }

  SECTION("multiple handlers"){
    bridge.received.push(vectString{"data3", "data2", "data1", "event"});
    vectString receivedMessage_1, receivedMessage_2;
    hub.on("event", [&receivedMessage_1](vectString message){
      receivedMessage_1 = message;
    });
    hub.on("event", [&receivedMessage_2](vectString message){
      receivedMessage_2 = message;
    });
    hub.poll();
    REQUIRE( receivedMessage_1 == vectString{"data3", "data2", "data1"} );
    REQUIRE( receivedMessage_2 == vectString{"data3", "data2", "data1"} );
  }
}
