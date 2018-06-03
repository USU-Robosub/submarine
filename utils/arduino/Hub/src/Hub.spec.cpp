#include <catch2/catch.hpp>
#include <Hub.hpp>
#include <Controller.mock.hpp>

#include <iostream>

TEST_CASE("hub can read and execute a controller", "[hub]"){
  Serial.$reset();
  int32_t buffer[3] = {0};
  Controller** controllers = new Controller*[1];
  Mock::Controller controller(buffer);
  controllers[0] = &controller;
  Hub hub(controllers, 1);
  Serial.$loadBuffer(0, 0);
  Serial.$loadBuffer(0, 1);
  Serial.$loadBuffer(3, 2);
  Serial.$loadBuffer(-40, 3);
  Serial.$loadBuffer(765, 4);
  Serial.$loadBuffer(13, 5);
  Serial.$bufferLength(6);
  hub.poll();
  REQUIRE( buffer[0] == -40 );
  REQUIRE( buffer[1] == 765 );
  REQUIRE( buffer[2] == 13 );
}

TEST_CASE("hub can emit an event", "[hub]"){
  Serial.$reset();
  Hub hub(nullptr, 0);
  int32_t data[5] = {-172, 127, 0, -5000, -50000};
  hub.emit(42, data, 5);
  REQUIRE( Serial.$nextInt() == 0 );
  REQUIRE( Serial.$nextInt() == 42 );
  REQUIRE( Serial.$nextInt() == 5 );
  REQUIRE( Serial.$nextInt() == -172 );
  REQUIRE( Serial.$nextInt() == 127 );
  REQUIRE( Serial.$nextInt() == 0 );
  REQUIRE( Serial.$nextInt() == -5000 );
  REQUIRE( Serial.$nextInt() == -50000 );
}
