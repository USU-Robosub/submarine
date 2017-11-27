#include "catch.hpp"
#include "Device/Motor.h"
#include "Device/Motor.mock.h"

TEST_CASE("the motor interface can be used", "[Motor]"){
  Device::Mock::TestMotor testMotor;
  Device::Motor* motor = &testMotor;
  motor->setSpeed(10);
  REQUIRE( testMotor.didSetSpeed );
}
