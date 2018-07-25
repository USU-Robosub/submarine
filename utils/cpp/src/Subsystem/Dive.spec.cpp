#include <catch.hpp>
#include <Comm/Hub.mock.hpp>
#include <Subsystem/Dive.hpp>
#include <Comm/tools.hpp>

TEST_CASE("Dive links an agent to an arduino", "[Dive]"){
  Comm::Mock::Hub<int> arduino;
  Comm::Mock::Hub<std::string> agent;
  Subsystem::Dive dive(&arduino, 42, &agent, "dive");

  SECTION("was added"){
    REQUIRE( agent.handlers.size() == 2 );
  }

  SECTION("was named correctly"){
    REQUIRE( agent.handlers[0].first == "dive/power" );
    REQUIRE( agent.handlers[1].first == "dive/steering" );
  }

  SECTION("dive/power can be set"){
    agent.handlers[0].second({"-1.234"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(-1.234f), Comm::floatAsInt32(0)} );
  }

  SECTION("dive/steering sets steering"){
    agent.handlers[1].second({"0.253"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(0.0f), Comm::floatAsInt32(0.253f)} );
  }
}
