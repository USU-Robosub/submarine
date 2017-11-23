#include "catch.hpp"
#include "PID_Controller.h"

TEST_CASE("PID_Controller parameters", "[PID_Controller]") {
  PID_Controller controller;

  SECTION("can be retrieved") {
    REQUIRE(controller.p == 0);
    REQUIRE(controller.d == 0);
    REQUIRE(controller.i == 0);
    REQUIRE(controller.setPoint == 0);
  }

  SECTION("can be set") {
    controller.p = 5;
    controller.d = 2.2;
    controller.i = 4.12;
    controller.setPoint = -5;

    REQUIRE(controller.p == 5);
    REQUIRE(controller.d == 2.2);
    REQUIRE(controller.i == 4.12);
    REQUIRE(controller.setPoint == -5);
  }
}

TEST_CASE("PID_Controller update function", "[PID_Controller]"){
  PID_Controller controller;

  SECTION("only proportional") {
    SECTION("setup #1") {
      controller.p = 5;
      controller.setPoint = -5;

      REQUIRE(controller.update(10.1) == -75.5);
      REQUIRE(controller.update(10.1) == -75.5);
    }
    SECTION("setup #2") {
      controller.p = -0.5;
      controller.setPoint = 1.2;

      REQUIRE(controller.update(6) == 2.4);
      REQUIRE(controller.update(6) == 2.4);
    }
  }

  SECTION("only integral") {
    SECTION("setup #1") {
      controller.i = 0.5;
      controller.setPoint = -3;

      REQUIRE(controller.update(4) == -3.5);
      REQUIRE(controller.update(9) == -9.5);
      REQUIRE(controller.update(-4) == -9);
    }
    SECTION("setup #2") {
      controller.i = -3;
      controller.setPoint = 0;

      REQUIRE(controller.update(0) == 0);
      REQUIRE(controller.update(5) == 15);
      REQUIRE(controller.update(-3) == 6);
    }
  }

  SECTION("only derivative"){
    SECTION("setup #1"){
      controller.d = 7.5;
      controller.setPoint = -1;

      REQUIRE(controller.update(1) == -15);
      REQUIRE(controller.update(2) == -7.5);
      REQUIRE(controller.update(3) == -7.5);
    }
    SECTION("setup #2"){
      controller.d = -3;
      controller.setPoint = -4.5;

      REQUIRE(controller.update(-4.5) == 0);
      REQUIRE(controller.update(5) == 28.5);
      REQUIRE(controller.update(0) == -15);
    }
  }
}
