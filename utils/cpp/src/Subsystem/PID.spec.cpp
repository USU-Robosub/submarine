#include <catch.hpp>
#include <Comm/Hub.mock.hpp>
#include <Subsystem/PID.hpp>
#include <Comm/tools.hpp>

TEST_CASE("PID links an agent to an arduino", "[PID]"){
  Comm::Mock::Hub<int> arduino;
  Comm::Mock::Hub<std::string> agent;
  Subsystem::PID pid(&arduino, 42, &agent, "pid");

  SECTION("was added"){
    REQUIRE( agent.handlers.size() == 1 );
  }

  SECTION("was named correctly"){
    REQUIRE( agent.handlers[0].first == "pid/setpoint" );
  }

  SECTION("setpoint can be set"){
    agent.handlers[0].second({"-1.234"});
    REQUIRE( arduino.messages.size() == 1 );
    REQUIRE( arduino.messages[0].first == 42 );
    REQUIRE( arduino.messages[0].second == std::vector<int>{Comm::floatAsInt32(-1.234f)} );
  }
}
