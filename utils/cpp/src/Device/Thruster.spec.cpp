#include "catch.hpp"
#include "Device/Thruster.h"
#include "Comm/Stream.mock.h"
#include "Comm/Hub.mock.h"

TEST_CASE("thruster sends power set to hub as integer", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  thruster.enable();

  thruster.setPower(0.5);
  REQUIRE( hub.writeQueue[1] == 300 ); // 0 - 200 - 400
}

TEST_CASE("thruster implements the motor interface", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  Device::Motor* motor = &thruster;
  thruster.enable();

  motor->setPower(-0.1);
  REQUIRE( hub.writeQueue[1] == 180 );
}

TEST_CASE("thruster limits power to -1 <= x <= 1", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  thruster.enable();

  SECTION("limits to x => -1"){
    thruster.setPower(-1.5);
    REQUIRE( hub.writeQueue[1] == 0 );
  }

  SECTION("limits to x => 1"){
    thruster.setPower(1.2);
    REQUIRE( hub.writeQueue[1] == 400 );
  }
}

TEST_CASE("thruster locks hub when set power", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);
  thruster.enable();

  SECTION("locks hub"){
    REQUIRE( hub.didLock == false );
    thruster.setPower(0.3);
    REQUIRE( hub.didLock == true );
  }

  SECTION("unlocks hub"){
    REQUIRE( hub.didLock == false );
    thruster.setPower(0.3);
    REQUIRE( hub.didUnlock == true );
  }
}

TEST_CASE("thruster sends id of thruster to set power of", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 50);
  thruster.enable();

  thruster.setPower(-0.7);
  REQUIRE( hub.writeQueue[0] == 50 );
}

TEST_CASE("thruster does not send power when disabled", "[Thruster]"){
  Comm::TestHub hub;
  Device::Thruster thruster(&hub, 0);

  SECTION("is disabled when created"){
    thruster.setPower(0.5);
    REQUIRE( hub.writeQueue.size() == 0 );
  }

  SECTION("can be disabled"){
    thruster.enable();
    thruster.disable();
    thruster.setPower(0.7);
    REQUIRE( hub.writeQueue.size() == 0 );
  }
}
