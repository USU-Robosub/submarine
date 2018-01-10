#include <catch.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/Stream.mock.hpp>

TEST_CASE("TCP bridge can write a message", "[TCP_Bridge]"){
  Comm::Mock::Stream<std::string> stream;
  Comm::TCP::Bridge bridge(&stream);

  SECTION("single message"){
    bridge.send(std::vector<std::string>{"a", "ab", "abc", "test"});
    REQUIRE( stream.pushed == std::vector<std::string>{
      "0", // integrity check
      "test", // name
      "3", // size of message
      "a", "ab", "abc" // message
    } );
  }

  SECTION("multiple messages"){
    bridge.send(std::vector<std::string>{"abc", "ab", "b", "tset"});
    bridge.send(std::vector<std::string>{"a", "b", "c", "d", "e", "f", "message"});
    REQUIRE( stream.pushed == std::vector<std::string>{
      "0", "tset", "3", "abc", "ab", "b",
      "0", "message", "6", "a", "b", "c", "d", "e", "f"
    } );
  }
}

TEST_CASE("TCP bridge can read a message", "[TCP_Bridge]"){
  Comm::Mock::Stream<std::string> stream;
  Comm::TCP::Bridge bridge(&stream);

  SECTION("single message"){
    stream.polled = std::vector<std::string>{"0", "test", "3", "a", "b", "c"};
    std::queue<std::vector<std::string>> messages = bridge.receive();
    REQUIRE( messages.size() == 1 );
    REQUIRE( messages.front() == std::vector<std::string>{"a", "b", "c", "test"} );
  }

  SECTION("multiple messages"){
    stream.polled = std::vector<std::string>{
      "0", "test", "3", "1", "2", "3",
      "0", "message", "2", "abc", "def",
      "0", "event", "0"
    };
    std::queue<std::vector<std::string>> messages = bridge.receive();
    REQUIRE( messages.size() == 3 );
    REQUIRE( messages.front() == std::vector<std::string>{"1", "2", "3", "test"} );
    messages.pop();
    REQUIRE( messages.front() == std::vector<std::string>{"abc", "def", "message"} );
    messages.pop();
    REQUIRE( messages.front() == std::vector<std::string>{"event"} );
  }
}
