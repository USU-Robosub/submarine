#include <catch.hpp>
#include <Comm/Hub.mock.hpp>
#include <Subsystem/Power.hpp>
#include <Comm/tools.hpp>

TEST_CASE("Power links an agent to an arduino", "[Power]"){
  Comm::Mock::Hub<int> arduino;
  Comm::Mock::Hub<std::string> agent;
  Subsystem::Power dive(&arduino, 42, &agent, "power");

  SECTION("was added"){
    REQUIRE( agent.handlers.size() == 1 );
  }

  SECTION("was named correctly"){
    REQUIRE( agent.handlers[0].first == "power/enable" );
  }

  SECTION("power/enable can be enabled"){
    agent.handlers[0].second({"1"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{1} );
  }

  SECTION("power/enable can be disabled"){
    agent.handlers[0].second({"0"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{0} );
  }
}
