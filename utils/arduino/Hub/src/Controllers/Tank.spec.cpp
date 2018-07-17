#include <catch2/catch.hpp>
#include <Controllers/Tank.hpp>
#include <Components/Motors/Motor.mock.hpp>
#include <tools.hpp>

TEST_CASE("Tank going forward or backwards sets both motors to same power", "[dive]"){
  Mock::Components::Motors::Motor left, right;
  Controllers::Tank tank(&left, &right);
  int32_t data[2] = { floatAsInt32(0.843f), floatAsInt32(0.0f) };
  tank.unfreeze();
  tank.execute(nullptr, data, 2);
  REQUIRE( left.$power == 0.843f );
  REQUIRE( right.$power == 0.843f );
}

TEST_CASE("Tank going left or right sets both motors to opposite power", "[dive]"){
  Mock::Components::Motors::Motor left, right;
  Controllers::Tank tank(&left, &right);
  int32_t data[2] = { floatAsInt32(0.0f), floatAsInt32(0.352f) };
  tank.unfreeze();
  tank.execute(nullptr, data, 2);
  REQUIRE( left.$power == 0.352f );
  REQUIRE( right.$power == -0.352f );
}

TEST_CASE("Tank can mix directions", "[dive]"){
  Mock::Components::Motors::Motor left, right;
  Controllers::Tank tank(&left, &right);
  int32_t data[2] = { floatAsInt32(1.0f), floatAsInt32(0.5f) };
  tank.unfreeze();
  tank.execute(nullptr, data, 2);
  REQUIRE( left.$power == 1.5f );
  REQUIRE( right.$power == 0.5f );
}

TEST_CASE("Tank enables and disables motors when frozen", "[dive]"){
  Mock::Components::Motors::Motor left, right;
  Controllers::Tank tank(&left, &right);
  REQUIRE_FALSE( left.$isEnabled );
  REQUIRE_FALSE( right.$isEnabled );
  tank.unfreeze();
  REQUIRE( left.$isEnabled );
  REQUIRE( right.$isEnabled );
  tank.freeze();
  REQUIRE_FALSE( left.$isEnabled );
  REQUIRE_FALSE( right.$isEnabled );
}
