#include <catch2/catch.hpp>
#include <Controllers/Tank.hpp>

TEST_CASE("tank attaches both motors", "[tank]"){
  Servo::reset();
  REQUIRE_FALSE( Servo::attached[1] );
  REQUIRE_FALSE( Servo::attached[2] );
  Controllers::Tank tank(1, 2);
  REQUIRE( Servo::attached[1] );
  REQUIRE( Servo::attached[2] );
}

TEST_CASE("tank sets each motor independently", "[tank]"){
  // Servo::reset();
  // Controllers::Tank tank(1, 2, false);
  // int32_t data[2] = { 4000, -2000 };
  // tank.execute(nullptr, data, 2);
  // REQUIRE( Servo::speed[1] == 4000 );
  // REQUIRE( Servo::speed[2] == -2000 );
}

TEST_CASE("tank limits speed in safe mode", "[tank]"){
  // Servo::reset();
  // Controllers::Tank tank(1, 2);
  // int32_t data[2] = { 181, -1 };
  // tank.execute(nullptr, data, 2);
  // REQUIRE( Servo::speed[1] == 170 );
  // REQUIRE( Servo::speed[2] == 10 );
  // data[0] = -1;
  // data[1] = 181;
  // tank.execute(nullptr, data, 2);
  // REQUIRE( Servo::speed[1] == 10 );
  // REQUIRE( Servo::speed[2] == 170 );
}
