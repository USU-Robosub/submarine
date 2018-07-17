#include <catch2/catch.hpp>
#include <Controllers/Dive.hpp>
#include <Components/Motors/Motor.mock.hpp>
#include <tools.hpp>

TEST_CASE("dive sets both motors to same speed", "[dive]"){
  Mock::Components::Motors::Motor front, back;
  Controllers::Dive dive(&front, &back);
  int32_t data[1] = { floatAsInt32(0.78) };
  dive.unfreeze();
  dive.execute(nullptr, data, 1);
  REQUIRE( front.$power == 0.78f );
  REQUIRE( back.$power == 0.78f );
}

TEST_CASE("dive enables and disables motors when frozen", "[dive]"){
  Mock::Components::Motors::Motor front, back;
  Controllers::Dive dive(&front, &back);
  REQUIRE_FALSE( front.$isEnabled );
  REQUIRE_FALSE( back.$isEnabled );
  dive.unfreeze();
  REQUIRE( front.$isEnabled );
  REQUIRE( back.$isEnabled );
  dive.freeze();
  REQUIRE_FALSE( front.$isEnabled );
  REQUIRE_FALSE( back.$isEnabled );
}
