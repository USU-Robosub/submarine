#include "catch.hpp"
#include "PID/Controller.h"

TEST_CASE("the pid controller can be created", "[PID_Controller]"){
  PID::Controller controller(1, 2, 3, 4);
  REQUIRE( controller.pGain == 1 );
  REQUIRE( controller.iGain == 2 );
  REQUIRE( controller.dGain == 3 );
  REQUIRE( controller.setPoint == 4 );
}

TEST_CASE("the pid controller works with just proportional gain", "[PID_Controller]"){
  SECTION("setup #1"){
    PID::Controller controller(1, 0, 0, 0);
    double newValue = 5;
    double deltaTime = 2;
    REQUIRE( controller.correctFor(newValue, deltaTime) == -5 );
  }
  SECTION("setup #2"){
    PID::Controller controller(3, 0, 0, 2);
    double newValue = 4;
    double deltaTime = 1;
    REQUIRE( controller.correctFor(newValue, deltaTime) == -6 );
  }
}

TEST_CASE("the pid controller works with just integral gain", "[PID_Controller]"){
  SECTION("setup #1"){
    PID::Controller controller(0, 1, 0, 0);
    double newValue = 5;
    double deltaTime = 2;
    REQUIRE( controller.correctFor(newValue, deltaTime) == -10 );
    REQUIRE( controller.correctFor(newValue, deltaTime) == -20 );
  }
  SECTION("setup #2"){
    PID::Controller controller(0, 3, 0, 2);
    double newValue = 4;
    double deltaTime = 1;
    REQUIRE( controller.correctFor(newValue, deltaTime) == -6 );
    REQUIRE( controller.correctFor(newValue, deltaTime) == -12 );
  }
}

TEST_CASE("the pid controller works with just differential gain", "[PID_Controller]"){
  SECTION("setup #1"){
    PID::Controller controller(0, 0, 1, 0);
    double newValue = 5;
    double deltaTime = 2;
    REQUIRE( controller.correctFor(newValue, deltaTime) == (5.0 / (-2.0)) );
    REQUIRE( controller.correctFor(newValue, deltaTime) == 0 );
  }
  SECTION("setup #2"){
    PID::Controller controller(0, 0, 3, 2);
    double newValue = 4;
    double deltaTime = 1;
    REQUIRE( controller.correctFor(newValue, deltaTime) == -6 );
    REQUIRE( controller.correctFor(newValue, deltaTime) == 0 );
  }
}

TEST_CASE("pid controller multi-variable test", "[PID_Controller]"){
  PID::Controller controller(1, 2, 3, 4);
  // sweep positive
  REQUIRE( controller.correctFor(1, 2) == 19.5 );
  REQUIRE( controller.correctFor(2, 2) == 20.5 );
  REQUIRE( controller.correctFor(3, 2) == 23.5 );
  REQUIRE( controller.correctFor(4, 2) == 22.5 );
  REQUIRE( controller.correctFor(5, 2) == 17.5 );
  REQUIRE( controller.correctFor(6, 2) == 8.5 );
  REQUIRE( controller.correctFor(7, 2) == -4.5 );
  // sweep back negative
  REQUIRE( controller.correctFor(7, 1) == -9 );
  REQUIRE( controller.correctFor(6, 1) == -9 );
  REQUIRE( controller.correctFor(5, 1) == -10 );
  REQUIRE( controller.correctFor(4, 1) == -9 );
  REQUIRE( controller.correctFor(3, 1) == -6 );
  REQUIRE( controller.correctFor(2, 1) == -1 );
  REQUIRE( controller.correctFor(1, 1) == 6 );
}
