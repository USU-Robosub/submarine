#include <catch.hpp>
#include <Comm/Serial/Bridge.hpp>
#include <Comm/Stream.mock.hpp>

TEST_CASE("serial bridge can write a message", "[SerialBridge]"){
  Comm::Mock::Stream<int> stream;
  Comm::Serial::Bridge bridge(&stream);

  SECTION("single message"){
    bridge.send(std::vector<int>{1, 2, 3, 4});
    REQUIRE( stream.pushed == std::vector<int>{
      0, // integrity check
      4, // name
      3, // size of message
      1, 2, 3 // message
    } );
  }

  SECTION("multiple messages"){
    bridge.send(std::vector<int>{4, 3, 2, 1});
    bridge.send(std::vector<int>{5, 6, 7, 8, 9, 10});
    REQUIRE( stream.pushed == std::vector<int>{
      0, 1, 3, 4, 3, 2,
      0, 10, 5, 5, 6, 7, 8, 9
    } );
  }
}

TEST_CASE("serial bridge can read a message", "[SerialBridge]"){
  Comm::Mock::Stream<int> stream;
  Comm::Serial::Bridge bridge(&stream);

  SECTION("single message"){
    stream.polled = std::vector<int>{0, 1, 3, 4, 3, 2};
    std::queue<std::vector<int>> messages = bridge.receive();
    REQUIRE( messages.size() == 1 );
    REQUIRE( messages.front() == std::vector<int>{4, 3, 2, 1} );
  }

  SECTION("multiple messages"){
    stream.polled = std::vector<int>{
      0, 1, 3, 4, 3, 2,
      0, 5, 1, 6,
      0, 6, 0
    };
    std::queue<std::vector<int>> messages = bridge.receive();
    REQUIRE( messages.size() == 3 );
    REQUIRE( messages.front() == std::vector<int>{4, 3, 2, 1} );
    messages.pop();
    REQUIRE( messages.front() == std::vector<int>{6, 5} );
    messages.pop();
    REQUIRE( messages.front() == std::vector<int>{6} );
  }
}
