#include "catch.hpp"
#include "Device/Thruster.h"
#include "Comm/Stream.mock.h"
#include "Comm/Hub.mock.h"

TEST_CASE("thruster sends power set to hub as integer", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  thruster.setPower(0.5);
  REQUIRE( hub.writeQueue.top() == 300 ); // 0 - 200 - 400
}

TEST_CASE("thruster implements the motor interface", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  Device::Motor* motor = &thruster;
  motor->setPower(-0.1);
  REQUIRE( hub.writeQueue.top() == 180 );
}

TEST_CASE("thruster limits power to -1 <= x => 1", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  thruster.setPower(-1.5);
  REQUIRE( hub.writeQueue.top() == 0 );
  thruster.setPower(1.2);
  REQUIRE( hub.writeQueue.top() == 400 );
}

TEST_CASE("thruster locks hub when set power", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  REQUIRE( hub.didLock == false );
  REQUIRE( hub.didUnlock == false );
  thruster.setPower(0.3);
  REQUIRE( hub.didLock == true );
  REQUIRE( hub.didUnlock == true );
}

TEST_CASE("thruster sends id of thruster to set power of", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 50);
  thruster.setPower(-0.7);
  hub.writeQueue.pop();
  REQUIRE( hub.writeQueue.top() == 50 );
}
