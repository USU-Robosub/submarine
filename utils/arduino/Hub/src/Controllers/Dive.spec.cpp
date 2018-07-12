// #include <catch2/catch.hpp>
// #include <Controllers/Dive.hpp>
//
// TEST_CASE("dive attaches both motors", "[dive]"){
//   Servo::$reset();
//   REQUIRE_FALSE( Servo::attached[1] );
//   REQUIRE_FALSE( Servo::attached[2] );
//   Controllers::Dive dive(1, 2);
//   REQUIRE( Servo::attached[1] );
//   REQUIRE( Servo::attached[2] );
// }
//
// TEST_CASE("dive sets both motors to same speed", "[dive]"){
//   Servo::$reset();
//   Controllers::Dive dive(1, 2, false);
//   int32_t data[1] = { 4000 };
//   dive.unfreeze();
//   dive.execute(nullptr, data, 1);
//   REQUIRE( Servo::speed[1] == 4000 );
//   REQUIRE( Servo::speed[2] == 4000 );
// }
//
// TEST_CASE("dive limits speed in safe mode", "[dive]"){
//   Servo::$reset();
//   Controllers::Dive dive(1, 2);
//   int32_t data[1] = { 181 };
//   dive.unfreeze();
//   dive.execute(nullptr, data, 1);
//   REQUIRE( Servo::speed[1] == 170 );
//   REQUIRE( Servo::speed[2] == 170 );
//   data[0] = -1;
//   dive.execute(nullptr, data, 1);
//   REQUIRE( Servo::speed[1] == 10 );
//   REQUIRE( Servo::speed[2] == 10 );
// }
