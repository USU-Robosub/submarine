#include <catch.hpp>
#include <Comm/Hub.mock.hpp>
#include <Subsystem/Tank.hpp>
#include <Comm/tools.hpp>

TEST_CASE("Tank links an agent to an arduino", "[Dive]"){
  Comm::Mock::Hub<int> arduino;
  Comm::Mock::Hub<std::string> agent;
  Subsystem::Tank tank(&arduino, 42, &agent, "tank");

  SECTION("provides 4 handlers"){
    REQUIRE( agent.handlers.size() == 4 );
  }

  SECTION("handlers are named correctly"){
    REQUIRE( agent.handlers[0].first == "tank/throttle" );
    REQUIRE( agent.handlers[1].first == "tank/steering" );
    REQUIRE( agent.handlers[2].first == "tank/left" );
    REQUIRE( agent.handlers[3].first == "tank/right" );
  }

  SECTION("tank/throttle sets throttle"){
    agent.handlers[0].second({"-0.126"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(-0.126f), Comm::floatAsInt32(0.0f)} );
  }

  SECTION("tank/steering sets steering"){
    agent.handlers[1].second({"0.253"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(0.0f), Comm::floatAsInt32(0.253f)} );
  }

  SECTION("tank/left sets left"){
    agent.handlers[2].second({"0.748"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(0.748f / 2.0f), Comm::floatAsInt32(0.748f / 2.0f)} );
  }

  SECTION("tank/right sets right"){
    agent.handlers[3].second({"-0.928"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(-0.928f / 2.0f), Comm::floatAsInt32(0.928f / 2.0f)} );
  }
}
