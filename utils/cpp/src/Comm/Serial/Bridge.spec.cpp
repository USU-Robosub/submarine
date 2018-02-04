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

  SECTION("partial message"){
    SECTION("[0][...]"){
      stream.polled = std::vector<int>{
        0
      };
    }
    SECTION("[0, name][...]"){
      stream.polled = std::vector<int>{
        0, 10
      };
    }
    SECTION("[0, name, length][...]"){
      stream.polled = std::vector<int>{
        0, 10, 3
      };
    }
    SECTION("[0, name, length, data][data...]"){
      stream.polled = std::vector<int>{
        0, 10, 3, 2, 3
      };
    }
    std::queue<std::vector<int>> messages = bridge.receive();
    REQUIRE( messages.size() == 0 );
  }

  SECTION("partial message continued"){
    SECTION("[0, name][length, data]"){
      stream.polled = std::vector<int>{
        0, 42
      };
      std::queue<std::vector<int>> messages = bridge.receive();
      stream.polled = std::vector<int>{
        0, 42, 4, 1, 2, 3, 4, 0, 11, 0, 0
      };
      messages = bridge.receive();
      REQUIRE( messages.size() == 2 );
      REQUIRE( messages.front() == std::vector<int>{1, 2, 3, 4, 42} );
      messages.pop();
      REQUIRE( messages.front() == std::vector<int>{11} );
      stream.polled = std::vector<int>{
        0, 42, 4, 1, 2, 3, 4, 0, 11, 0, 0, -1, 1, 1000
      };
      messages = bridge.receive();
      REQUIRE( messages.size() == 1 );
      REQUIRE( messages.front() == std::vector<int>{1000, -1});
    }
  }
}
